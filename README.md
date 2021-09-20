# RayTracer

![RayTracer](https://github.com/T-rvw/RayTracer/actions/workflows/main.yml/badge.svg?branch=main)

Easy to make and build in different platforms powered by [Premake](https://github.com/premake/premake-core).

Well organized example projects to help you understand.

## Build

1. `git clone https://github.com/T-rvw/RayTracer.git`
2. click and open `RayTracer` folder
3. open a bash window and input build command

Windows + VS2019 : `./premake5 vs2019`

Linux + gmake : `./premake5 gmake2`

Mac + xcode : `./premake5 xcode4`

If you want more build options, please see [Premake document](https://premake.github.io/docs/Using-Premake).

## Performance

By default, we opened OpenMP compile option to support parallel calculation in multiple threads.

Also, please use release build to render complex images to save your time.

## Reference links

[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

[Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)