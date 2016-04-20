# raytracing

## Schedule
1. 首先实现Camera类, 用最朴素的光线跟踪能看到一个球.. Done
2. 完善Camera, Bitmap, World
    - Camera, Bitmap封装成只能用接口函数访问
    - 实现World类, 能缓存计算中间结果, 并持久化
3. Camera
    - ~~Camera类添加物距, 平面镜头~~
    - ~~添加球体的反射光线跟踪~~

4. 颜色
    - 叠加效果不太好, 尤其是亮度低的地方, 影子
4. Known Bugs
    - ~~遮挡光源无效, 但是遮挡镜头有效~~
    - ~~颜色叠加溢出~~