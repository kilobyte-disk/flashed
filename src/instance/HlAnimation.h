#ifndef HLANIMATION_H
#define HLANIMATION_H

struct HlAnimation {
	char name[20]; /* Max 20 char name */

	int frame_rate;

	int frames;
	int frame_data[20][4]; /* Max 20 frames */
	/* Frame data is stored in this order:
	 * [frame][0] == x
	 * [frame][1] == y
	 * [frame][2] == width
	 * [frame][3] == height
	 * This information should be used to build a source_rect
	 * each frame for a given spritesheet.
	 */
};

extern struct HlAnimation HLANIM_FromStarlingXML(char *path, char *anim_prefix, char *name);


#endif
