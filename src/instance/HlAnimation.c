/*
 * kilobyte-disk 2025
 * HlAnimation.c
 *
 * An animation manager to be used with HlSprite.
 * Supports Starling XML animations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include "raylib.h"

#include "HlAnimation.h"
#include "HlSprite.h"

/* Convert a string "123" into an int 123. */
static int GetAttrInt(xmlAttr *attr)
{
	char attr_str[5];
	char *endptr;
	
	strncpy(attr_str, attr->children->content, 5);
	attr_str[5] = '\0';

	int converted = (int) strtol(attr_str, &endptr, 10);

	return converted;
}

/* Parse an XML file in starling format
 * into an HlAnimation struct, that describes
 * the size of each frame in a dynamic spritesheet.
 */
struct HlAnimation HLANIM_FromStarlingXML(char *xml_path, char *anim_prefix, char *name)
{
	printf( "[HlAnimation]: Getting animation from Starling XML... prefix: %s\n",
								anim_prefix);

	struct HlAnimation animation;
	animation.frames = -1;

	/* Name can be 20 chars max */
	int anim_name_len = strlen(name);
	if (anim_name_len > 20) {
		anim_name_len = 20;
		printf("[HlAnimation]: (WARNING) Animation name %s is longer than 20 chars! This will be truncated!\n", name);
	}

	strncpy(animation.name, name, 20);
	animation.name[anim_name_len] = '\0';


	/* Begin XML parsing */	

	xmlDoc *document;
	document = xmlReadFile(xml_path, NULL, 0);

	xmlNode *root, *first_child, *node;
	root = xmlDocGetRootElement(document);
	first_child = root->children;

	for (node = first_child; node; node = node->next) {

		/* The first attr is always the name of the animation frame */
		xmlAttr *attr = node->properties;
		
		if (!attr) {
			continue;
		}

		/* Compare current prefix to the target prefix */
		int prefix_length = strlen(anim_prefix);
		int attr_length = strlen(attr->children->content);
		
		char cur_attr_prefix[prefix_length];
		
		strncpy(cur_attr_prefix, attr->children->content, prefix_length);
		cur_attr_prefix[prefix_length] = '\0'; 

		printf("[HlAnimation]: Current node prefix: %s\n", cur_attr_prefix);

		if (strcmp(cur_attr_prefix, anim_prefix) != 0) {
			continue;
		}


		/* Parse XML after attr prefix and anim_prefix match */
		printf("[HlAnimation]: success! match found!\n");

		/* Increment the frame count */
		animation.frames++;

		int attrs_found = 0;

		while (attr) {

			/* Parse and assign values */
			if (strcmp(attr->name, "x") == 0) {
				animation.frame_data[animation.frames][1] = GetAttrInt(attr);
				attrs_found++;
			}
			if (strcmp(attr->name, "y") == 0) {
				animation.frame_data[animation.frames][2] = GetAttrInt(attr);
				attrs_found++;
			}
			if (strcmp(attr->name, "width") == 0) {
				animation.frame_data[animation.frames][3] = GetAttrInt(attr);
				attrs_found++;
			}
			if (strcmp(attr->name, "height") == 0) {
				animation.frame_data[animation.frames][4] = GetAttrInt(attr);
				attrs_found++;
			}

			attr = attr->next;
		}

		printf("[HlAnimation]: Found (%i/4) attrs for frame %i\n", attrs_found, animation.frames);
	}

	if (animation.frames < 0) {
		printf(
			"[HlAnimation]: [ERROR]: Unable to locate animation frame data for XML: (%s). Using prefix: (%s)\n",
			xml_path,
			anim_prefix
		);
	}

	xmlFreeDoc(document);

	/* Configure animation defaults */
	/* NOTE: WTF? These values don't stay unless declared here...*/
	animation.frame_rate = 24;

	return animation;
}
