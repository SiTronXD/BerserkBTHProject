#version 130

uniform sampler2D u_screenTexture;

uniform float u_percentage;

void main()
{
	vec2 uv = gl_TexCoord[0].xy; 
	vec4 col = texture2D(u_screenTexture, uv);

	// Relative luminance
	vec4 desaturatedCol = vec4(vec3(0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b), col.a);

	float coolDownPart = uv.y < 1.0f - u_percentage ? 1.0 : 0.0;
	vec4 finalCol = mix(col, desaturatedCol, coolDownPart);

	gl_FragColor = finalCol;
}