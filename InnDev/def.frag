#version 450 core
// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 normal;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform vec3 lightDir;

void main(){
	vec3	nNormal		= normalize(normal);
	vec3	nLightDir	= normalize(lightDir); 

	color = texture( myTextureSampler, UV ).rgb * max(dot(nLightDir, nNormal), 0.0);
    //color = texture( myTextureSampler, UV ).rgb * max(dot(normal, normalize(-lightDir)),0.0);
}