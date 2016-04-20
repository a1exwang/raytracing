//
// Created by alexwang on 4/18/16.
//

#include "camera.h"
#include "config.h"
#include "stdio.h"
#include <yaml.h>


Vector parse_vector_string(char *str) {
  Vector ret;
  int s = 0;
  int last = -1;
  for (int i = 0; ; ++i) {

    // the first number
    if (str[i] >= '0' && str[i] <= '9' && last == -1) {
      last = i;
    }
    else if (str[i] == ',' || str[i] == 0) {
      char backup = str[i];
      str[i] = 0;
      if (s == 0 && last != -1) {
        ret.x = atof(&str[last]);
      }
      else if (s == 1 && last != -1) {
        ret.y = atof(&str[last]);
      }
      else if (s == 2 && last != -1) {
        ret.z = atof(&str[last]);
        return ret;
      }
      else {
        fprintf(stderr, "yaml format error\n");
        abort();
      }
      s++;
      last = -1;
      str[i] = backup;
    }
  }
}

#define OBJECT_KIND_START(type, obj) \
  if (mapping_started && strcmp(obj_kind, "") == 0) { \
    strcpy(obj_kind, value); \
  }\
  else if (strcmp(obj_kind, #obj) == 0) { \
    type object = obj; \

#define OBJECT_KIND_END(obj) }

#define PARSE_VECTOR(name) \
  if (strcmp(value, #name) == 0) { \
    current_kind = #name; \
  } \
  else if (strcmp(current_kind, #name) == 0) { \
    object->name = parse_vector_string(value); \
    current_kind = ""; \
  }

#define PARSE_DOUBLE(name) \
  if (strcmp(value, #name) == 0) { \
    current_kind = #name; \
  } \
  else if (strcmp(current_kind, #name) == 0) { \
    object->name = atof(value); \
    current_kind = ""; \
  }

#define PARSE_INT(name) \
  if (strcmp(value, #name) == 0) { \
    current_kind = #name; \
  } \
  else if (strcmp(current_kind, #name) == 0) { \
    object->name = atoi(value); \
    current_kind = ""; \
  }


void read_configuration(const char *file, Camera *camera, World *world) {
  FILE *fh = fopen(file, "r");
  yaml_parser_t parser;

  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  yaml_event_t  event;
  char *value;
  const char *current_kind = "";
  char obj_kind[100]; obj_kind[0] = 0;
  int mapping_started = 0;

  do {
    if (!yaml_parser_parse(&parser, &event)) {
      printf("Parser error %d\n", parser.error);
      exit(EXIT_FAILURE);
    }

    switch(event.type) {
    case YAML_NO_EVENT: puts("No event!"); break;
      /* Stream read_configuration/end */
    //case YAML_STREAM_START_EVENT: puts("STREAM START"); break;
    //case YAML_STREAM_END_EVENT:   puts("STREAM END");   break;
      /* Block delimeters */
    //case YAML_DOCUMENT_START_EVENT: puts("<b>Start Document</b>"); break;
    //case YAML_DOCUMENT_END_EVENT:   puts("<b>End Document</b>");   break;
    case YAML_SEQUENCE_START_EVENT: puts("<b>Start Sequence</b>"); break;
    case YAML_SEQUENCE_END_EVENT:   puts("<b>End Sequence</b>");   break;
    case YAML_MAPPING_START_EVENT:
      mapping_started = 1;
      break;
    case YAML_MAPPING_END_EVENT:
      mapping_started = 0;
      obj_kind[0] = 0;
      break;
      /* Data */
    //case YAML_ALIAS_EVENT:  printf("Got alias (anchor %s)\n", event.data.alias.anchor); break;
    case YAML_SCALAR_EVENT:
      value = (char*) event.data.scalar.value;

      OBJECT_KIND_START(Camera*, camera)
        PARSE_INT(width);
        PARSE_INT(height);
        PARSE_VECTOR(pos);
        PARSE_VECTOR(up);
        PARSE_VECTOR(front);
        PARSE_DOUBLE(viewport_width);
        PARSE_DOUBLE(viewport_height);
        PARSE_DOUBLE(xmin);
        PARSE_DOUBLE(xmax);
        PARSE_DOUBLE(ymin);
        PARSE_DOUBLE(ymax);
        PARSE_DOUBLE(zmin);
        PARSE_DOUBLE(zmax);
      OBJECT_KIND_END(camera)

      mapping_started = 0;

      //printf("Got scalar (value %s)\n", event.data.scalar.value);
      break;
    }
    if(event.type != YAML_STREAM_END_EVENT)
      yaml_event_delete(&event);
  } while(event.type != YAML_STREAM_END_EVENT);

  /* Cleanup */
  yaml_parser_delete(&parser);
  fclose(fh);
}