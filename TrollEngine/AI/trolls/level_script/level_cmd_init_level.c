#include "level_script_headers.h"

void level_cmd_init_level(void) {
    init_graph_node_start(NULL, (struct GraphNodeStart *) &gObjParentGraphNode);
    
	level_init_intercept(); // AI/post_object_load_pass.c

    sCurrentCmd = CMD_NEXT;
}
