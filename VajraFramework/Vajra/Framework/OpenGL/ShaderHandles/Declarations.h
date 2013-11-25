#ifndef SHADER_HANDLES_DECLARATIONS_H
#define SHADER_HANDLES_DECLARATIONS_H

#include <string>

/*
 * NOTE: When any of the following enums are modified, the corresponding getter & setter functions must be modified too:
 */
// TODO [Implement] Add a better way of handling named enums

enum Shader_variable_qualifier_t {
	SHADER_VARIABLE_QUALIFIER_attribute,
	SHADER_VARIABLE_QUALIFIER_uniform,
	SHADER_VARIABLE_QUALIFIER_varying,
	SHADER_VARIABLE_QUALIFIER_invalid,
};

enum Shader_variable_datatype_t {
	SHADER_VARIABLE_DATATYPE_vec2,
	SHADER_VARIABLE_DATATYPE_vec3,
	SHADER_VARIABLE_DATATYPE_vec4,
	SHADER_VARIABLE_DATATYPE_mat3,
	SHADER_VARIABLE_DATATYPE_mat4,
	SHADER_VARIABLE_DATATYPE_invalid,
};

enum Shader_variable_variablename_id_t {
	SHADER_VARIABLE_VARIABLENAME_vPosition,
	SHADER_VARIABLE_VARIABLENAME_vNormal,
	SHADER_VARIABLE_VARIABLENAME_uvCoords_in,
	SHADER_VARIABLE_VARIABLENAME_uvCoords_out,
	SHADER_VARIABLE_VARIABLENAME_mvpMatrix,
	SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix,
	SHADER_VARIABLE_VARIABLENAME_lightIntensity,
	//
	SHADER_VARIABLE_VARIABLENAME_DLight0Direction,
	SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor,
	SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor,
	SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor,
	//
	SHADER_VARIABLE_VARIABLENAME_invalid,
};

Shader_variable_qualifier_t GetShaderVariableQualifierFromString(std::string s);
Shader_variable_datatype_t GetShaderVariableDatatypeFromString(std::string s);
Shader_variable_variablename_id_t GetShaderVariableVariableNameIdFromString(std::string s);

std::string GetStringForShaderVariableQualifier(Shader_variable_qualifier_t t);
std::string GetStringForShaderVariableDatatype(Shader_variable_datatype_t t);
std::string GetStringForShaderVariableVariableNameId(Shader_variable_variablename_id_t t);

#endif // SHADER_HANDLES_DECLARATIONS_H