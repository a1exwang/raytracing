//
// Created by alexwang on 4/21/16.
//

#include "ray_trace.h"
#include "world.h"

// 获得所有光源, 在物体某个位置的光照颜色之和
Vector get_light_color(const World *world, const Vector *pos, const Object *object) {
  Vector total = world->ambient_light;
  LIST_FOREACH(&world->first_light->list, Light, list, light)
  Vector color = light->diffuse_func(light, world, pos, object);
  total = color_mix(total, color);
  LIST_FOREACH_END()
  return total;
}

void ray_trace_set_ambient(RayTrace *trace, RayTrace **diffuse) {
  (*diffuse) = (RayTrace*) malloc(sizeof(RayTrace));
  (*diffuse)->world = trace->world;
  (*diffuse)->type = RT_Ambient;
  (*diffuse)->parent = trace;
  (*diffuse)->x = trace->x;
  (*diffuse)->y = trace->y;
  (*diffuse)->trace_times = trace->trace_times - 1;
  (*diffuse)->color = trace->world->ambient_light;
}

/* map: ray ==> (reflection, refraction, diffusion) */
void ray_trace_mapper(RayTrace *trace, RayTrace **rt_reflect, RayTrace **rt_refract, RayTrace **rt_diffusion) {
  *rt_reflect = *rt_refract = *rt_diffusion = NULL;

  if (trace->trace_times <= 0) {
    ray_trace_set_ambient(trace, rt_diffusion);
    return;
  }

  Vector intersection, n;
  Ray reflect;
  // 该光线直接射中的物体
  Object *object = world_closest_object(trace->world, &trace->ray, &intersection, &reflect, &n);
  if (!object) {
    ray_trace_set_ambient(trace, rt_diffusion);
    return;
  }

  Vector total, att;
  // 光源在此处的散射光线, 以及环境光
  Vector light_color = get_light_color(trace->world, &intersection, object);
  total = color_mix(light_color, trace->world->ambient_light);
  total.x *= trace->att.x;
  total.y *= trace->att.y;
  total.z *= trace->att.z;

  RayTrace *rt_diffuse = malloc(sizeof(RayTrace));
  rt_diffuse->parent = trace;
  rt_diffuse->type = RT_Diffuse;
  rt_diffuse->world = trace->world;
  rt_diffuse->x = trace->x;
  rt_diffuse->y = trace->y;
  rt_diffuse->color = total;
  *rt_diffusion = rt_diffuse;

  // 如果是反射材质, 跟踪反射光线
  if (object->attenuation_func) {
    *rt_reflect = malloc(sizeof(RayTrace));
    (*rt_reflect)->parent = trace;
    (*rt_reflect)->type = RT_Reflection;
    (*rt_reflect)->trace_times = trace->trace_times - 1;
    (*rt_reflect)->world = trace->world;
    (*rt_reflect)->x = trace->x;
    (*rt_reflect)->y = trace->y;
    (*rt_reflect)->ray = reflect;
    (*rt_reflect)->color = total;
    (*rt_reflect)->att = object->attenuation_func(object, &trace->ray, &reflect, &intersection, &n);
    (*rt_reflect)->att.x *= trace->att.x;
    (*rt_reflect)->att.y *= trace->att.y;
    (*rt_reflect)->att.z *= trace->att.z;
    // 这里可以优化, 如果att小于一定数值则不再跟踪
  }

  if (object->refraction_func) {
    // 如果是折射材质, 跟踪折射光线
    Ray refraction;
    if (object->refraction_func(object, &trace->ray, &reflect, &intersection, &n, &refraction, &att)) {
      *rt_refract = malloc(sizeof(RayTrace));
      (*rt_refract)->parent = trace;
      (*rt_refract)->type = RT_Refraction;
      (*rt_refract)->trace_times = trace->trace_times - 1;
      (*rt_refract)->world = trace->world;
      (*rt_refract)->x = trace->x;
      (*rt_refract)->y = trace->y;
      (*rt_refract)->ray = refraction;
      (*rt_refract)->color = total;
      (*rt_refract)->att = att;
      (*rt_refract)->att.x *= trace->att.x;
      (*rt_refract)->att.y *= trace->att.y;
      (*rt_refract)->att.z *= trace->att.z;
    }
  }
}

/* reduce: (reflection, refractioin) => parent(may be reflection or refraction) */
Vector ray_trace_reducer(const Vector *ra, const Vector *rb) {
  Vector ret = color_mix(*ra, *rb);
  return ret;
}

Vector ray_trace_map_reduce_driver(const World *world, const Ray *ray, int total_trace_times, int x, int y) {
  RayTrace *trace = malloc(sizeof(RayTrace));
  trace->type = RT_Root;
  trace->world = world;
  trace->att.x = trace->att.y = trace->att.z = 1;
  trace->trace_times = total_trace_times;
  trace->ray = *ray;
  trace->parent = NULL;
  trace->x = x;
  trace->y = y;

  // enqueue trace
  ListNode queue;
  ListNode leaves;
  list_node_init(&queue);
  list_node_init(&leaves);

  LIST_NODE_INSERT(&queue, trace);

  while (1) {
    // break if queue empty?
    if (list_node_is_empty(&queue))
      break;

    // dequeue
    RayTrace *rt;
    LIST_NODE_REMOVE_LAST(&queue, RayTrace, rt);

    // map
    RayTrace *refl, *refr, *dif;
    ray_trace_mapper(rt, &refl, &refr, &dif);

    // enqueue all unmapped, and save all leaf nodes
    if (refl != NULL) {
      if (refr->type == RT_Reflection) {
        LIST_NODE_INSERT(&queue, refl);
      }
      else {
        LIST_NODE_INSERT(&queue, refl);
      }
    }

    if (refr != NULL) {
      if (refr->type == RT_Refraction) {
        LIST_NODE_INSERT(&queue, refr);
      }
      else {
        LIST_NODE_INSERT(&leaves, refr);
      }
    }

    if (dif != NULL) {
      LIST_NODE_INSERT(&leaves, dif);
    }
  }

  Vector color = color_black();
  while (1) {
    if (list_node_is_empty(&leaves)) {
      break;
    }
    // dequeue
    RayTrace *rt;
    LIST_NODE_REMOVE_LAST(&leaves, RayTrace, rt);

    color = ray_trace_reducer(&rt->color, &color);
  }
  return color;
}

// 递归跟踪某一条光线
void ray_trace(World *world, Ray *ray, int trace_times, int x, int y, Vector **buffer) {
  buffer[x][y] = ray_trace_map_reduce_driver(world, ray, trace_times, x, y);
}
/*
Vector ray_trace_shabby(World *world, Ray *ray, int trace_times) {
  Vector total = world->ambient_light;
  if (trace_times <= 0)
    return total;

  Vector intersection, n;
  Ray reflect;
  // 该光线直接射中物体
  Object *object = world_closest_object(world, ray, &intersection, &reflect, &n);
  if (!object)
    return total;

  // 光源在此处的散射光线
  Vector light_color = get_light_color(world, &intersection, object);
  total = color_mix(light_color, world->ambient_light);

  Vector color, att;
  // 如果是反射材质, 跟踪反射光线
  if (object->attenuation_func) {
    color = ray_trace(world, &reflect, trace_times - 1, 0, 0, NULL);
    // 乘以反射光线的衰减系数
    att = object->attenuation_func(object, ray, &reflect, &intersection, &n);
    color.x *= att.x;
    color.y *= att.y;
    color.z *= att.z;
    total = color_mix(color, total);
  }

  if (object->refraction_func) {
    // 如果是折射材质, 跟踪折射光线
    Ray refraction;
    if (object->refraction_func(object, ray, &reflect, &intersection, &n, &refraction, &att)) {
      color = ray_trace(world, &refraction, trace_times - 1, 0, 0, NULL);
      color.x *= att.x;
      color.y *= att.y;
      color.z *= att.z;
      total = color_mix(color, total);
    }
  }

  return total;
}*/