/*
 * kilobyte-disk 2025
 * alpha.fs
 *
 * Purpose:
 * An OpenGL fragment shader to set alpha values when rendering things.
 * Simply, just a transparency modifier
 */

#version 330

/* Input vertext attributes (from vertex shader) */

in vec2 fragTexCoord;
in vec4 fragColor;

/* Input uniform values (from raylib) */
uniform sampler2D texture0;
uniform vec4 colDiffuse;

/* Output fragment color */
out vec4 finalColor;


/* Add custom variables here */
/* uniform keyworded variables can be modified at runtime by other scripts */

uniform float alpha_mod = 1.0f;

void main()
{
	/* Texel color fetching from texture sampler */
	vec4 texelColor = texture(texture0, fragTexCoord);

	/* Modify alpha by alpha_mod */
	texelColor.a = texelColor.a * alpha_mod;

	/* Final color is the color from the texture
	 * times the tint color (colDiffuse)
	 * times the fragment color (interpolated vertex color)
	 */
	finalColor = texelColor * colDiffuse * fragColor;
}
