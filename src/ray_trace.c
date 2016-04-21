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
void ray_trace_once(RayTrace *trace, RayTrace **rt_reflect, RayTrace **rt_refract, RayTrace **rt_diffusion) {
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
// one of refraction and reflection must exist
//RayTrace *ray_trace_combine(RayTrace *rt_reflect, RayTrace *rt_refract, int *done) {
//  RayTrace *parent = NULL;
//  if (rt_reflect) {
//    parent = rt_reflect->parent;
//  }
//  else {
//    parent = rt_refract->parent;
//  }
//
//  if (!parent) {
//    *done = 1;
//  }
//
//
//  return parent;
//}

// 递归跟踪某一条光线
Vector ray_trace(World *world, Ray *ray, int trace_times) {
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
    color = ray_trace(world, &reflect, trace_times - 1);
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
      color = ray_trace(world, &refraction, trace_times - 1);
      color.x *= att.x;
      color.y *= att.y;
      color.z *= att.z;
      total = color_mix(color, total);
    }
  }

  return total;
}