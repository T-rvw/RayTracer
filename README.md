# RayTracer

![Windows](https://github.com/T-rvw/RayTracer/actions/workflows/Windows.yml/badge.svg?branch=main) ![MacOS](https://github.com/T-rvw/RayTracer/actions/workflows/MacOS.yml/badge.svg?branch=main)

Easy to make and build in different platforms powered by [Premake](https://github.com/premake/premake-core).

Well organized example projects to help you understand.

## Build

`git clone https://github.com/T-rvw/RayTracer.git`

Windows + VS2019 : run `generate_vs2019.bat`.

MacOS + XCode : run `generate_xcode.command`. Note : `chmod +x generate_xcode.command` before the first run.

Other platforms & IDEs : See [Premake document](https://premake.github.io/docs/Using-Premake).

## Performance

By default, we opened OpenMP compile option to support parallel calculation in multiple threads.

Also, please use release build to render complex images to save your time.

## Reference links

[Mathematics for 3D Game Programming and Computer Graphics, Third Edition](http://www.mathfor3dgameprogramming.com)

[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

[Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)

[Ray Tracing: The Rest Of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
