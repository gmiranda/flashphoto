@echo off
Rem Intel(R) C++ Compiler Build Environment for 32-bit applications

echo.
echo Intel(R) C++ Compiler 8.0 Build Environment for 32-bit applications
echo Copyright (C) 1985-2003 Intel Corporation. All rights reserved.
echo.

@call "C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\Bin\Vcvars32.bat"

echo.

SET INTEL_COMPILER80=C:\Program Files\Intel\CPP\Compiler80

SET INTEL_SHARED=C:\Program Files\Fichiers communs\Intel\Shared Files

SET INTEL_LICENSE_FILE=C:\Program Files\Fichiers communs\Intel\Licenses

SET PATH=%INTEL_COMPILER80%\Ia32\Bin;%INTEL_SHARED%\Ia32\Bin;%PATH%

SET LIB=%INTEL_COMPILER80%\Ia32\Lib;%INTEL_SHARED%\Ia32\Lib;%LIB%

SET INCLUDE=%INTEL_COMPILER80%\Ia32\Include;%INCLUDE%

cd ../../examples
echo.
echo ** Compiling 'CImg_test'
echo.
icl /Ox CImg_test.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'fade_images'
echo.
icl /Ox fade_images.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'greycstoration'
echo.
icl /Ox greycstoration.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'hough_transform'
echo.
icl /Ox hough_transform.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'inrcast'
echo.
icl /Ox inrcast.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'odykill'
echo.
icl /Ox odykill.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'mcf_levelsets'
echo.
icl /Ox mcf_levelsets.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'image_registration'
echo.
icl /Ox image_registration.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'image2ascii'
echo.
icl /Ox image2ascii.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'pde_heatflow2D'
echo.
icl /Ox pde_heatflow2D.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'pde_TschumperleDeriche2D'
echo.
icl /Ox pde_TschumperleDeriche2D.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'pslider'
echo.
icl /Ox pslider.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'render3d'
echo.
icl /Ox render3d.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'tutorial'
echo.
icl /Ox tutorial.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'tetris'
echo.
icl /Ox tetris.cpp gdi32.lib user32.lib
echo.
echo ** Compiling 'wavelet_atrous'
echo.
icl /Ox wavelet_atrous.cpp gdi32.lib user32.lib
cd ..
