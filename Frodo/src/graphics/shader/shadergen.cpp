#include "shader.h"
#include <core/log.h>
#include <stdarg.h>

#define CMP_VARS(op, p1, p2) \
if (ShaderGenIsVariableDefined(para1, type)) { \
 \
	float32 var1 = ShaderGenGetVariableInternal(para1, type)->data; \
	float32 var2 = 0; \
	if (ShaderGenIsVariableDefined(para2, type)) { \
		var2 = ShaderGenGetVariableInternal(para2, type)->data; \
	} else { \
		var2 = (float32)atof(*para2); \
	} \
	 \
	if (var1 op var2) return true; \
	\
} else { \
	FD_WARNING("[ShaderGen] Variable \"%s\" not defined", *para1); \
	return false; \
} \

static String arithmetic_chars(" =<>+-*/");

	//TODO: Loggin everywhere
static String sg_endblock("#shaderGen endblock");
static String sg_define_r("#shaderGen define_r ");
static String sg_define_b("#shaderGen define_b ");
static String sg_define("#shaderGen define ");
static String sg_if("#shaderGen if ");
static String sg_elif("#shaderGen elif ");
static String sg_else("#shaderGen else");
static String sg_endif("#shaderGen endif");
static String sg_generate("#shaderGen generate ");
static String sg_shadergen("#shaderGen ");
static String sg_add("sg_add");
static String sg_sub("sg_sub");
static String sg_mul("sg_mul");
static String sg_div("sg_div");

static void RemoveDecimalsInString(String& source) {
	uint_t start = source.Find(".");

	if (start == (uint_t)-1) return;

	source.Remove(start, source.length);
}

static bool ShaderGenProcessForLoopFunction(float32 variable, float32 end, FD_SHADER_GEN_FUNCTION_TYPE op) {
	switch (op) {
		case FD_EQ:
			if (variable == end) return true;
			break;
		case FD_NEQ:
			if (variable != end) return true;
			break;
		case FD_LS:
			if (variable < end) return true;
			break;
		case FD_GR:
			if (variable > end) return true;
			break;
		case FD_LE:
			if (variable <= end) return true;
			break;
		case FD_GE:
			if (variable >= end) return true;
			break;
		default:
			FD_FATAL("[ShaderGen] Unknown function in for loop \"%u\"", (uint32)op);
			return false;
	}

	return false;
}

static void InsertString(String& source, const String& code, uint_t offset) {
	String afterBlock = source.SubString(offset, source.length);
	source.Remove(offset, source.length);

	source.Append(code);
	source.Append(afterBlock);
}

Shader::ShaderGenVariable* Shader::ShaderGenGetVariableInternal(const String& name, FD_SHADER_TYPE type) {
	uint_t size = variables.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderGenVariable* variable = variables.Get(i);
		if (type == variable->shader && variable->name == name) return variable;
	}

	return nullptr;
}

bool Shader::ShaderGenIsBlockDefined(const String& name, FD_SHADER_TYPE type) {
	uint_t size = blocks.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderGenBlock* block = blocks.Get(i);
		if (block->shader == type && block->name == name) return true;
	}

	return false;
}

void Shader::ShaderGenParseDefinitions(String& source, FD_SHADER_TYPE type) {

	//define
	while (true) {
		while_beginning1:
		uint_t start = source.Find(sg_define);

		if (start == (uint_t)-1) break;

		uint_t nameStart = start + sg_define.length;
		uint_t nameEnd = source.Find(" ", nameStart+1);
		uint_t end = source.Find("\n", nameEnd);
		String data = source.SubString(nameEnd, end).RemoveBlankspace();

		float32 value = (float32)atof(*data);

		ShaderGenVariable* variable = new ShaderGenVariable;
		variable->shader = type;
		variable->data = value;
		variable->name = source.SubString(nameStart, nameEnd).RemoveBlankspace();

		source.Remove(start, end);

		for (uint_t i = 0; i < variables.GetSize(); i++) {
			if (variables[i]->name == variable->name && variables[i]->shader == type) {
				FD_WARNING("[ShaderGen] Variable \"%s\" re-definition, saving existing", *variable->name);
				delete variable;
				goto while_beginning1;
			}
		}

		variables.Push_back(variable);

		FD_DEBUG("[ShaderGen] Added variable <NAME: %s DATA: %f>", *variable->name, value);
	}

	//define_r
	while (true) {
		while_beginning2:
		uint_t start = source.Find(sg_define_r);

		if (start == (uint_t)-1) break;

		uint_t nameStart = start + sg_define_r.length;
		uint_t nameEnd = source.Find("\n", nameStart);
		uint_t end = source.Find("\n", nameEnd);

		ShaderGenVariable* variable = new ShaderGenVariable;
		variable->shader = type;
		variable->data = 0;
		variable->name = source.SubString(nameStart, nameEnd).RemoveBlankspace();

		source.Remove(start, end);

		for (uint_t i = 0; i < variables.GetSize(); i++) {
			if (variables[i]->name == variable->name && variables[i]->shader == type) {
				FD_WARNING("[ShaderGen] Variable \"%s\" re-definition, saving existing", *variable->name);
				delete variable;
				goto while_beginning2;
			}
		}

		variables.Push_back(variable);
		FD_DEBUG("[ShaderGen] Added variable <NAME: %s DATA: NOT_SET>", *variable->name);
	}

	//define_b
	while (true) {
		while_beginning3:
		uint_t start = source.Find(sg_define_b);

		if (start == (uint_t)-1) break;

		uint_t nameStart = start + sg_define_b.length;
		uint_t nameEnd = source.Find("\n", nameStart);
		uint_t blockEnd = source.Find(sg_endblock, nameEnd);
		uint_t end = blockEnd + sg_endblock.length;

		ShaderGenBlock* block = new ShaderGenBlock;
		block->shader = type;
		block->name = source.SubString(nameStart, nameEnd);
		block->code = source.SubString(nameEnd, blockEnd);

		source.Remove(start, end);

		for (uint_t i = 0; i < blocks.GetSize(); i++) {
			if (blocks[i]->name == block->name && blocks[i]->shader == type) {
				FD_WARNING("[ShaderGen] Block \"%s\" re-definition, saving existing", *block->name);
				delete block;
				goto while_beginning3;
			}
		}

		blocks.Push_back(block);

		FD_DEBUG("[ShaderGen] Added block <NAME: %s>", *block->name);
	}
}

void Shader::ShaderGenProcessConditions(String& source, FD_SHADER_TYPE type) {
	while (true) {
		uint_t start = source.Find(sg_if);

		if (start == (uint_t)-1) break;

		uint_t functionStart = start + sg_if.length;
		uint_t functionEnd = source.Find("\n", functionStart);

		String function = source.SubString(functionStart, functionEnd).RemoveBlankspace();

		bool res = ShaderGenProcessFunction(function, type);

		if (res) {
			uint_t end = source.Find(sg_endif, start);

			if (end == (uint_t)-1) {
				FD_FATAL("[ShaderGen] No \"endif\" found after \"if\" aborting");
				return;
			}

			uint_t other = source.Find(sg_shadergen, functionEnd);

			if (other == end) {
				source.Remove(end, end+sg_endif.length);
				source.Remove(start, functionEnd);
			} else {
				source.Remove(other, end + sg_endif.length);
				source.Remove(start, functionEnd);
			}
		} else {
			uint_t end = source.Find(sg_endif, start);

			if (end == (uint_t)-1) {
				FD_FATAL("[ShaderGen] No \"endif\" found after \"if\" aborting");
				return;
			}

			uint_t firstElif = source.Find(sg_elif, functionEnd);
			uint_t firstElse = source.Find(sg_else, functionEnd);

			if (firstElse == firstElif) {
				source.Remove(start, end + sg_endif.length);
			} else {
				List<uint_t> elifs;

				if (firstElif != (uint_t)-1) {
					uint_t elif = firstElif;
					while (elif < end && elif != (uint_t)-1) {
						elifs.Push_back(elif);
						elif = source.Find(sg_elif, elif + sg_elif.length);
					}
				}

				elifs.Push_back(firstElse);

				bool result = false;

				for (uint_t i = 0; i < elifs.GetSize()-1; i++) {
					uint_t elif = elifs[i];
					uint_t functionStart = elif + sg_elif.length;
					uint_t functionEnd = source.Find("\n", functionStart);

					String function = source.SubString(functionStart, functionEnd).RemoveBlankspace();

					result = ShaderGenProcessFunction(function, type);

					uint_t next = elifs[i + 1];
					if (result) {

						if (next != (uint_t)-1) {
							source.Remove(next, end + sg_endif.length);
							source.Remove(start, functionEnd);
						} else {
							source.Remove(end, end + sg_endif.length);
							source.Remove(start, functionEnd);
						}

						break;
					} else {
						if (next != (uint_t)-1) {
							continue;
						} else {
							source.Remove(end, end + sg_endif.length);
							source.Remove(start, functionEnd);
							result = true;
						}
					}
				}

				if (!result) {
					source.Remove(end, end + sg_endif.length);
					source.Remove(start, firstElse + sg_else.length);
				}
			}
			
		}
	}
}

void Shader::ShaderGenProcessGeneration(String& source, FD_SHADER_TYPE type) {

	while (true) {
		uint_t start = source.Find(sg_generate);

		if (start == (uint_t)-1) break;

		uint_t functionStart = start + sg_generate.length;
		uint_t functionEnd = source.Find(")", functionStart)+1;
		uint_t nameEnd = source.Find("\n", functionEnd);

		String function = source.SubString(functionStart, functionEnd);
		String blockName = source.SubString(functionEnd, nameEnd).RemoveBlankspace();

		source.Remove(start, nameEnd);

		if (!ShaderGenIsBlockDefined(blockName, type)) {
			FD_FATAL("[ShaderGen] Block \"%s\" not defined", *blockName);
			continue;
		}
		
		String code = ShaderGenGetBlock(blockName, type);

		if (function == "single") {
			ShaderGenAddVariableData(code, type);
			InsertString(source, code, start);

		} else if (function.StartsWith("for(")) {
			String forVariableName, forStartName, forOpName, forEndName, forIncName;
			
			ShaderGenGetParametersFromFunction(function, 4, &forVariableName, &forStartName, &forOpName, &forEndName, &forIncName);

			bool isTempVar = false;
			String result;

			if (!ShaderGenIsVariableDefined(forVariableName, type)) {
				ShaderGenVariable* tmpVar = new ShaderGenVariable;

		
				tmpVar->name = forVariableName;
				tmpVar->shader = type;
				tmpVar->data = 0;
				isTempVar = true;

				variables.Push_back(tmpVar);
			}


			ShaderGenVariable* forVariable = ShaderGenGetVariableInternal(forVariableName, type);

			float32 forStart = 0, forEnd = 0, forInc = 0;
			FD_SHADER_GEN_FUNCTION_TYPE forOp = (FD_SHADER_GEN_FUNCTION_TYPE)0;

			if (ShaderGenIsVariableDefined(forStartName, type)) {
				forStart = ShaderGenGetVariableInternal(forStartName, type)->data;
			} else {
				forStart = (float32)atof(*forStartName);
				if (forStart == 0 && forStartName[0] != ' ') {
					FD_FATAL("[ShaderGen] Variable \"%s\" not defined", *forStartName);
				}
			}


			for (uint32 i = FD_EQ; i <= FD_LE; i++) {
				if (forOpName == GetFunctionTypeString((FD_SHADER_GEN_FUNCTION_TYPE)i)) forOp = (FD_SHADER_GEN_FUNCTION_TYPE)i;
			}

			if (forOp == (FD_SHADER_GEN_FUNCTION_TYPE)0) {
				FD_FATAL("[ShaderGen] %s is not a valid operation for a \"for\" loop", *forOpName);
				goto error;
			}

			if (ShaderGenIsVariableDefined(forEndName, type)) {
				forEnd = ShaderGenGetVariableInternal(forEndName, type)->data;
			} else {
				forEnd = (float32)atof(*forEndName);
				if (forEnd == 0 && forEndName[0] != ' ') {
					FD_FATAL("[ShaderGen] Variable \"%s\" not defined", *forEndName);
				}
			}

			if (ShaderGenIsVariableDefined(forIncName, type)) {
				ShaderGenVariable* var = ShaderGenGetVariableInternal(forIncName, type);
				forInc = var->data;
			} else {
				forInc = (float32)atof(*forIncName);
				if (forInc == 0 && forIncName[0] != ' ') {
					FD_FATAL("[ShaderGen] Variable \"%s\" not defined", *forIncName);
				}
			}
			
			forVariable->data = forStart;

			while (true) {

				if (ShaderGenProcessForLoopFunction(forVariable->data, forEnd, forOp)) {
					String newCode(code);

					ShaderGenProcessArithmeticOperations(newCode, type);
					ShaderGenAddVariableData(newCode, type);

					result += newCode;
				} else {
					break;
				}

				forVariable->data += forInc;
			}

		error:
			if (isTempVar) ShaderGenUndefVariable(forStartName, type);

			InsertString(source, result, start);
		}
	}

}

bool Shader::ShaderGenProcessFunction(String function, FD_SHADER_TYPE type) {
	if (function.StartsWith("defined(")) {
		return ShaderGenIsVariableDefined(function.SubString(8, function.Find(")", 9)).RemoveBlankspace(), type);
	}else if (function.StartsWith("true")) {
		return true;
	} else if (function.StartsWith("false")) {
		return false;
	} else if (function.StartsWith("eq(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(==, para1, para2)
		
	} else if (function.StartsWith("neq(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 4, &para1, &para2);

		CMP_VARS(!= , para1, para2)
			
	} else if (function.StartsWith("gr(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(>, para1, para2);

	} else if (function.StartsWith("ls(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(<, para1, para2)

	} else if (function.StartsWith("ge(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(>=, para1, para2)

	} else if (function.StartsWith("le(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(<=, para1, para2)
	}

	FD_WARNING("[ShaderGen] Unknown function \"%s\"", *function);
	return false;
}

bool Shader::ShaderGenIsVariableDefined(const String& name, FD_SHADER_TYPE type) {
	return ShaderGenGetVariableInternal(name, type) == nullptr ? false : true;
}

void Shader::ShaderGenGetParametersFromFunction(const String& function, uint_t offset...) {
	va_list list;
	va_start(list, offset);

	String parameters = function.SubString(offset, function.Find(")", offset+1)).RemoveBlankspace();

	uint_t comma = parameters.Find(",");
	uint_t numParameters = parameters.Count(",")+1;

	if (numParameters == 0) {
		FD_FATAL("[ShaderGen] Function \"%s\" only has 1 or less parameters, min 2 required", *function);
	}

	uint_t start = 0;

	for (uint_t i = 0; i < numParameters; i++) {
		String* para = va_arg(list, String*);
		if (comma == (uint_t)-1) comma = parameters.length;
		*para = parameters.SubString(start, comma).RemoveBlankspace();
		start = comma + 1;
		comma = parameters.Find(",", start);
	}

	va_end(list);
}

void Shader::ShaderGenAddVariableData(String& source, FD_SHADER_TYPE type) {
	uint_t size = variables.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderGenVariable* var = variables[i];
		if (var->shader != type) continue;

		while (true) {
			uint_t start = source.Find(*var->name);

			if (start == (uint_t)-1) break;

			uint_t length = var->name.length;

			source.Remove(start, start + length);

			char varValue[128];
			sprintf(varValue, "%f", var->data);

			String data(varValue);

			if (start == 0) goto skip_arithmetic_check;

			char charBefore[2];
			charBefore[0] = source[start - 1];
			charBefore[1] = 0;

			if (arithmetic_chars.Find(charBefore) == (uint_t)-1) RemoveDecimalsInString(data);
			
			skip_arithmetic_check:
			InsertString(source, data, start);
		}
	}
}

void Shader::ShaderGenProcessArithmeticOperations(String& source, FD_SHADER_TYPE type) {
	//add
	#define ARITH_OP(func, sig) \
	uint_t start = source.Find(sig); \
 \
	if (start == (uint_t)-1) break; \
 \
	uint_t end = source.Find(")", start)+1; \
 \
	String function = source.SubString(start, end); \
	source.Remove(start, end); \
 \
	String para1, para2; \
 \
	ShaderGenGetParametersFromFunction(function, sig.length + 1, &para1, &para2); \
 \
	if (ShaderGenIsVariableDefined(para1, type)) { \
		ShaderGenVariable* var1 = ShaderGenGetVariableInternal(para1, type); \
		float32 var2 = 0; \
 \
		if (ShaderGenIsVariableDefined(para2, type)) { \
			var2 = ShaderGenGetVariableInternal(para2, type)->data; \
		} else { \
			var2 = (float32)atof(*para2); \
		} \
 \
		float32 result = var1->data func var2; \
	 \
		char varValue[128]; \
		sprintf(varValue, "%f", result); \
 \
		String data(varValue); \
 \
		if (start != 0) { \
			char charBefore[2]; \
			charBefore[0] = source[start - 1]; \
			charBefore[1] = 0; \
 \
			if (arithmetic_chars.Find(charBefore) == (uint_t)-1) RemoveDecimalsInString(data); \
		} \
 \
		InsertString(source, data, start); \
		 \
 \
	} else { \
		FD_WARNING("[ShaderGen] Variable \"%s\" not defined", *para1); \
	}  \


	while (true) {
		ARITH_OP(+, sg_add)
	}

	while (true) {
		ARITH_OP(-, sg_sub)
	}

	while (true) {
		ARITH_OP(*, sg_mul)
	}

	while (true) {
		ARITH_OP(/, sg_div)
	}
}

void Shader::ShaderGenSetVariable(const String& name, FD_SHADER_TYPE type, float32 data) {
	uint_t size = variables.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderGenVariable* variable = variables[i];
		if (variable->shader == type && variable->name == name) {
			variable->data = data;
			FD_DEBUG("[ShaderGen] Variable \"%s\" updated. DATA: %f", *name, data);
			return;
		}
	}


	ShaderGenVariable* var = new ShaderGenVariable;
	var->name = name;
	var->shader = type;
	var->data = data;

	variables.Push_back(var);

	FD_DEBUG("[ShaderGen] Variable \"%s\" doesn't exist. Creating.... with DATA: %f", *name, data);
}

void Shader::ShaderGenUndefVariable(const String& name, FD_SHADER_TYPE type) {
	ShaderGenVariable* var = ShaderGenGetVariableInternal(name, type);
	if (var == nullptr) return;

	variables.Remove(var);

	delete var;

	FD_DEBUG("[ShaderGen] Undefined variable \"%s\"", *name);
}

float32 Shader::ShaderGenGetVariable(const String& name, FD_SHADER_TYPE type) {
	ShaderGenVariable* var = ShaderGenGetVariableInternal(name, type);
	if (var != nullptr) return var->data;
	FD_WARNING("[ShaderGen] Variable \"%s\" not found", *name);
	return 0;
}

String Shader::ShaderGenGetBlock(const String& name, FD_SHADER_TYPE type) {
	uint_t size = blocks.GetSize();
	for (uint_t i = 0; i < size; i++) {
		ShaderGenBlock* block = blocks.Get(i);
		if (block->shader == type && block->name == name) return block->code;
	}

	FD_WARNING("[ShaderGen] Block \"%s\" not found", *name);
	return String("Empty Block");
}

void Shader::ShaderGenComplete() {
	ShaderGenProcessConditions(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ShaderGenProcessConditions(pSource, FD_SHADER_TYPE_PIXELSHADER);

	ShaderGenProcessGeneration(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ShaderGenProcessGeneration(pSource, FD_SHADER_TYPE_PIXELSHADER);

	FD_DEBUG("[ShaderGen] Shader generation completed, compiling...");
	Compile(vSource, pSource);
}
