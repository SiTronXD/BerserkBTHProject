#version 130

uniform sampler2D u_screenTexture;

uniform vec2 u_resolution;

void main()
{
	vec3 col = texture2D(u_screenTexture, gl_FragCoord.xy / u_resolution).rgb;

	gl_FragColor = vec4(col, 1.0);
}