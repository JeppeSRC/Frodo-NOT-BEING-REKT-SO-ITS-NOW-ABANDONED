#include "shader.h"
#include <core/log.h>
#include <stdarg.h>

#define CMP_VARS(op, ret, p1, p2) \
if (ShaderGenIsVariableDefined(para1, type)) { \
	ShaderGenVariable* var1 = ShaderGenGetVariableInternal(para1, type);  \
	ShaderGenVariable* var2 = nullptr; \
	if (ShaderGenIsVariableDefined(para2, type)) { \
		var2 = ShaderGenGetVariableInternal(para2, type); \
	} else { \
		var2 = new ShaderGenVariable; \
		var2->name = String("tmpVar%") + para2; \
		var2->dataType = var1->dataType; \
		double value = atof(*para2); \
	 \
		if (var2->dataType == FD_U8) { var2->data = new unsigned char((unsigned char)value); } else if (var2->dataType == FD_S8) { var2->data = new signed char((signed char)value); } else if (var2->dataType == FD_U16) { var2->data = new unsigned short((unsigned short)value); } else if (var2->dataType == FD_S16) { var2->data = new signed short((signed short)value); } else if (var2->dataType == FD_U32) { var2->data = new unsigned int((unsigned int)value); } else if (var2->dataType == FD_S32) { var2->data = new signed int((signed int)value); } else if (var2->dataType == FD_U64) { var2->data = new unsigned long long((unsigned long long)value); } else if (var2->dataType == FD_S64) { var2->data = new signed long long((signed long long)value); } else if (var2->dataType == FD_F32) { var2->data = new float((float)value); } else if (var2->dataType == FD_F64) { var2->data = new double(value); } \
	} \
 \
	if (var1->dataType == FD_UNKNOWN) { \
		return false; \
	} \
	 \
	if (var1->dataType != var2->dataType) { \
		return false; \
	} \
	switch (var1->dataType) { \
		case FD_S8: \
		case FD_U8: \
				if ((*(unsigned char*)var1->data) op (*(unsigned char*)var2->data)) return ret; \
				break; \
		case FD_S16: \
		case FD_U16: \
				if ((*(unsigned short*)var1->data) op (*(unsigned short*)var2->data)) return ret; \
				break; \
		case FD_S32: \
		case FD_U32: \
				if ((*(unsigned int*)var1->data) op (*(unsigned int*)var2->data)) return ret; \
				break; \
		case FD_S64: \
		case FD_U64: \
				if ((*(unsigned long long*)var1->data) op (*(unsigned long long*)var2->data)) return ret; \
				break; \
		case FD_F32: \
				if ((*(float*)var1->data) op (*(float*)var2->data)) return ret; \
				break; \
		case FD_F64: \
				if ((*(double*)var1->data) op (*(double*)var2->data)) return ret; \
				break; \
		} \
} else { \
	return false; \
}  \

	//TODO: Loggin everywhere
static String sg_endblock("#shaderGen endblock");
static String sg_define_r("#shaderGen define_r ");
static String sg_define_b("#shaderGen define_b ");
static String sg_define("#shaderGen define ");
static String sg_if("#shaderGen if ");
static String sg_elif("#shaderGen elif ");
static String sg_else("#shaderGen else");
static String sg_endif("#shaderGen endif");
static String sg_shadergen("#shaderGen ");

static String sg_type_u8("u8");
static String sg_type_s8("s8");
static String sg_type_u16("u16");
static String sg_type_s16("s16");
static String sg_type_u32("u32");
static String sg_type_s32("s32");
static String sg_type_u64("u64");
static String sg_type_s64("s64");
static String sg_type_f32("f32");
static String sg_type_f64("f64");

Shader::ShaderGenVariable* Shader::ShaderGenGetVariableInternal(const String& name, FD_SHADER_TYPE type) {
	size_t size = variables.GetSize();
	for (size_t i = 0; i < size; i++) {
		ShaderGenVariable* variable = variables.Get(i);
		if (type == variable->shader && variable->name == name) return variable;
	}

	return nullptr;
}

void Shader::ShaderGenParseDefinitions(String& source, FD_SHADER_TYPE type) {

	//define
	while (true) {
		size_t start = source.Find(sg_define);

		if (start == (size_t)-1) break;

		size_t nameStart = start + sg_define.length;
		size_t nameEnd = source.Find(" ", nameStart+1);
		size_t typeEnd = source.Find(" ", nameEnd+1);
		size_t end = source.Find("\n", typeEnd);

		String dataType = source.SubString(nameEnd, typeEnd).RemoveBlankspace();
		String data = source.SubString(typeEnd, end).RemoveBlankspace();

		double value = atof(*data);

		ShaderGenVariable* variable = new ShaderGenVariable;
		variable->shader = type;
		
		if		(dataType == sg_type_u8)  { variable->dataType = FD_U8;  variable->data = new unsigned char((unsigned char)value); }
		else if (dataType == sg_type_s8)  { variable->dataType = FD_S8;  variable->data = new signed char((signed char)value); }
		else if (dataType == sg_type_u16) { variable->dataType = FD_U16; variable->data = new unsigned short((unsigned short)value); }
		else if (dataType == sg_type_s16) { variable->dataType = FD_S16; variable->data = new signed short((signed short)value); }
		else if (dataType == sg_type_u32) { variable->dataType = FD_U32; variable->data = new unsigned int((unsigned int)value); }
		else if (dataType == sg_type_s32) { variable->dataType = FD_S32; variable->data = new signed int((signed int)value); }
		else if (dataType == sg_type_u64) { variable->dataType = FD_U64; variable->data = new unsigned long long((unsigned long long)value); }
		else if (dataType == sg_type_s64) { variable->dataType = FD_S64; variable->data = new signed long long((signed long long)value); }
		else if (dataType == sg_type_f32) { variable->dataType = FD_F32; variable->data = new float((float)value); }
		else if (dataType == sg_type_f64) { variable->dataType = FD_F64; variable->data = new double(value); }
				
		variable->name = source.SubString(nameStart, nameEnd).RemoveBlankspace();

		source.Remove(start, end);

		variables.Push_back(variable);
	}

	//define_r
	while (true) {
		size_t start = source.Find(sg_define_r);

		if (start == (size_t)-1) break;

		size_t nameStart = start + sg_define_r.length;
		size_t nameEnd = source.Find("\n", nameStart);
		size_t end = source.Find("\n", nameEnd);

		ShaderGenVariable* variable = new ShaderGenVariable;
		variable->shader = type;
		variable->dataType = FD_UNKNOWN;
		variable->data = nullptr;
		variable->name = source.SubString(nameStart, nameEnd).RemoveBlankspace();

		source.Remove(start, end);

		variables.Push_back(variable);
	}

	//define_b
	while (true) {
		size_t start = source.Find(sg_define_b);

		if (start == (size_t)-1) break;

		size_t nameStart = start + sg_define_b.length;
		size_t nameEnd = source.Find("\n", nameStart);
		size_t blockEnd = source.Find(sg_endblock, nameEnd);
		size_t end = blockEnd + sg_endblock.length;

		ShaderGenBlock* block = new ShaderGenBlock;
		block->shader = type;
		block->name = source.SubString(nameStart, nameEnd);
		block->code = source.SubString(nameEnd, blockEnd);

		source.Remove(start, end);

		blocks.Push_back(block);

	}
}

void Shader::ShaderGenProcessConditions(String& source, FD_SHADER_TYPE type) {
	while (true) {
		size_t start = source.Find(sg_if);

		if (start == (size_t)-1) break;

		size_t functionStart = start + sg_if.length;
		size_t functionEnd = source.Find("\n", functionStart);

		String function = source.SubString(functionStart, functionEnd).RemoveBlankspace();

		bool res = ShaderGenProcessFunction(function, type);

		if (res) {
			size_t end = source.Find(sg_endif, start);

			if (end == (size_t)-1); //TODO: logging

			size_t other = source.Find(sg_shadergen, functionEnd);

			if (other == end) {
				source.Remove(end, end+sg_endif.length);
				source.Remove(start, functionEnd);
			} else {
				source.Remove(other, end + sg_endif.length);
				source.Remove(start, functionEnd);
			}
		} else {
			size_t end = source.Find(sg_endif, start);

			if (end == (size_t)-1); //TODO: error

			size_t firstElif = source.Find(sg_elif, functionEnd);
			size_t firstElse = source.Find(sg_else, functionEnd);

			if (firstElse == firstElif) {
				source.Remove(start, end + sg_endif.length);
			} else {
				List<size_t> elifs;

				if (firstElif != (size_t)-1) {
					size_t elif = firstElif;
					while (elif < end && elif != (size_t)-1) {
						elifs.Push_back(elif);
						elif = source.Find(sg_elif, elif + sg_elif.length);
					}
				}

				elifs.Push_back(firstElse);

				bool result = false;

				for (size_t i = 0; i < elifs.GetSize()-1; i++) {
					size_t elif = elifs[i];
					size_t functionStart = elif + sg_elif.length;
					size_t functionEnd = source.Find("\n", functionStart);

					String function = source.SubString(functionStart, functionEnd).RemoveBlankspace();

					result = ShaderGenProcessFunction(function, type);

					size_t next = elifs[i + 1];
					if (result) {

						if (next != (size_t)-1) {
							source.Remove(next, end + sg_endif.length);
							source.Remove(start, functionEnd);
						} else {
							source.Remove(end, end + sg_endif.length);
							source.Remove(start, functionEnd);
						}

						break;
					} else {
						if (next != (size_t)-1) {
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

		/*if (ShaderGenIsVariableDefined(para1, type)) {
				ShaderGenVariable* var1 = ShaderGenGetVariableInternal(para1, type); 
				ShaderGenVariable* var2 = nullptr;
				if (ShaderGenIsVariableDefined(para2, type)) {
					var2 = ShaderGenGetVariableInternal(para2, type);
				} else {
					var2 = new ShaderGenVariable;
					var2->name = String("tmpVar%") + para2;
					var2->dataType = var1->dataType;
					double value = atof(*para2);

					if		(var2->dataType == FD_U8)  { var2->data = new unsigned char((unsigned char)value); }
					else if (var2->dataType == FD_S8)  { var2->data = new signed char((signed char)value); }
					else if (var2->dataType == FD_U16) { var2->data = new unsigned short((unsigned short)value); }
					else if (var2->dataType == FD_S16) { var2->data = new signed short((signed short)value); }
					else if (var2->dataType == FD_U32) { var2->data = new unsigned int((unsigned int)value); }
					else if (var2->dataType == FD_S32) { var2->data = new signed int((signed int)value); }
					else if (var2->dataType == FD_U64) { var2->data = new unsigned long long((unsigned long long)value); }
					else if (var2->dataType == FD_S64) { var2->data = new signed long long((signed long long)value); }
					else if (var2->dataType == FD_F32) { var2->data = new float((float)value); }
					else if (var2->dataType == FD_F64) { var2->data = new double(value); }
				}
						
				if (var1->dataType == FD_UNKNOWN) {
					return false; 
				} 
							
				if (var1->dataType != var2->dataType) {
					return false; 
				} 
									
				switch (var1->dataType) {
				case FD_S8: 
				case FD_U8: 
						if ((*(unsigned char*)var1->data) == (*(unsigned char*)var2->data)) return true; 
						break; 
				case FD_S16: 
				case FD_U16: 
						if ((*(unsigned short*)var1->data) == (*(unsigned short*)var2->data)) return true; 
						break; 
				case FD_S32: 
				case FD_U32: 
						if ((*(unsigned int*)var1->data) == (*(unsigned int*)var2->data)) return true; 
						break; 
				case FD_S64: 
				case FD_U64: 
						if ((*(unsigned long long*)var1->data) == (*(unsigned long long*)var2->data)) return true; 
						break; 
				case FD_F32: 
						if ((*(float*)var1->data) == (*(float*)var2->data)) return true; 
						break; 
				case FD_F64: 
						if ((*(double*)var1->data) == (*(double*)var2->data)) return true; 
						break; 
				} 

				if (var2->name == (String("tmpVar%").Append(para2))) {
					delete var2->data;
					delete var2;
				}
											
		} else {
			return false; 
		}  */

		CMP_VARS(==, true, para1, para2)
		
	} else if (function.StartsWith("neq(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 4, &para1, &para2);

		CMP_VARS(!= , true, para1, para2)
				
			
	} else if (function.StartsWith("gr(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(> , true, para1, para2);

	} else if (function.StartsWith("ls(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(<, true, para1, para2)

	} else if (function.StartsWith("ge(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(>=, true, para1, para2)

	} else if (function.StartsWith("le(")) {
		String para1, para2;

		ShaderGenGetParametersFromFunction(function, 3, &para1, &para2);

		CMP_VARS(<=, true, para1, para2)
	}

	return false;
}

bool Shader::ShaderGenIsVariableDefined(const String& name, FD_SHADER_TYPE type) {
	return ShaderGenGetVariableInternal(name, type) == nullptr ? false : true;
}

void Shader::ShaderGenGetParametersFromFunction(const String& function, size_t offset...) {
	va_list list;
	va_start(list, offset);

	String parameters = function.SubString(offset, function.Find(")", offset+1)).RemoveBlankspace();

	size_t comma = parameters.Find(",");
	size_t numParameters = parameters.Count(",")+1;

	if (numParameters == 0); //TODO: Log

	size_t start = 0;

	for (size_t i = 0; i < numParameters; i++) {
		String* para = va_arg(list, String*);
		if (comma == (size_t)-1) comma = parameters.length;
		*para = parameters.SubString(start, comma).RemoveBlankspace();
		start = comma + 1;
		comma = parameters.Find(",", start);
	}

	va_end(list);
}

void Shader::ShaderGenSetVariable(const String& name, FD_SHADER_TYPE type, FD_SHADER_GEN_VARIABLE_TYPE variableType, void* data) {
	size_t size = variables.GetSize();
	for (size_t i = 0; i < size; i++) {
		ShaderGenVariable* variable = variables[i];
		if (variable->shader == type && variable->name == name) {
			variable->dataType = variableType;
			if (variable->data) delete variable->data;
			variable->data = data;
			return;
		}
	}

	ShaderGenVariable* var = new ShaderGenVariable;
	var->name = name;
	var->shader = type;
	var->dataType = variableType;
	var->data = data;

	variables.Push_back(var);
	//TODO: log
}

void Shader::ShaderGenUndefVariable(const String& name, FD_SHADER_TYPE type, bool deleteData) {
	ShaderGenVariable* var = ShaderGenGetVariableInternal(name, type);
	if (var == nullptr) return;

	if (deleteData) delete var->data;

	variables.Remove(var);

	delete var;
}

void*  Shader::ShaderGenGetVariable(const String& name, FD_SHADER_TYPE type) {
	ShaderGenVariable* var = ShaderGenGetVariableInternal(name, type);
	if (var != nullptr) return var->data;
	return nullptr;
}

String Shader::ShaderGenGetBlock(const String& name, FD_SHADER_TYPE type) {
	size_t size = blocks.GetSize();
	for (size_t i = 0; i < size; i++) {
		ShaderGenBlock* block = blocks.Get(i);
		if (block->shader == type && block->name == name) return block->code;
	}

	return String("Empty Block");
}

void Shader::ShaderGenComplete() {
	ShaderGenProcessConditions(vSource, FD_SHADER_TYPE_VERTEXSHADER);
	ShaderGenProcessConditions(pSource, FD_SHADER_TYPE_PIXELSHADER);

	Compile(vSource, pSource);
}
