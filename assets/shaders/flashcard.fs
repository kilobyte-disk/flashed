/*
* alpha_discard.fs
* Discards alpha when rendering textures with transparency.
* Also flips the texture being rendered. (Necessary because OpenGL is upside down)
*/

#version 330

/* Input vertex attributes (from vertex shader) */
in vec2 fragTexCoord;
in vec4 fragColor;

/* Input uniform values */
uniform sampler2D texture0;
uniform vec4 colDiffuse;

/* Output fragment color */
out vec4 finalColor;

/* Add your custom variables here */

void main()
{
	/* Texel color fetching from texture sampler */	


   	vec4 texelColor = texture(texture0, fragTexCoord);

   	/* Implement here your fragment shader code */
   	if (texelColor.a == 0.0) {
	  	discard;
   	}

   	/* final color is the color from the texture 
   	/*    times the tint color (colDiffuse)
   	/*    times the fragment color (interpolated vertex color)
    	*/
   	finalColor = texelColor*colDiffuse*fragColor;
}
