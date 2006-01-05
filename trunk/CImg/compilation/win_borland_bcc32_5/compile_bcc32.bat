@echo off
echo.
echo Compiling CImg with Borland BCC32 compiler
echo.

cd ../../examples
echo.
echo ** Compiling 'CImg_test'
echo.
bcc32 -w- -O CImg_test.cpp
echo.
echo ** Compiling 'fade_images'
echo.
bcc32 -w- -O fade_images.cpp
echo.
echo ** Compiling 'greycstoration'
echo.
bcc32 -w- -O greycstoration.cpp
echo.
echo ** Compiling 'hough_transform'
echo.
bcc32 -w- -O hough_transform.cpp
echo.
echo ** Compiling 'inrcast'
echo.
bcc32 -w- -O inrcast.cpp
echo.
echo ** Compiling 'odykill'
echo.
bcc32 -w- -O odykill.cpp
echo.
echo ** Compiling 'mcf_levelsets'
echo.
bcc32 -w- -O mcf_levelsets.cpp
echo.
echo ** Compiling 'image_registration'
echo.
bcc32 -w- -O image_registration.cpp
echo.
echo ** Compiling 'image2ascii'
echo.
bcc32 -w- -O image2ascii.cpp
echo.
echo ** Compiling 'pde_heatflow2D'
echo.
bcc32 -w- -O pde_heatflow2D.cpp
echo.
echo ** Compiling 'pde_TschumperleDeriche2D'
echo.
bcc32 -w- -O pde_TschumperleDeriche2D.cpp
echo.
echo ** Compiling 'pslider'
echo.
bcc32 -w- -O pslider.cpp
echo.
echo ** Compiling 'render3d'
echo.
bcc32 -w- -O render3d.cpp
echo.
echo ** Compiling 'tutorial'
echo.
bcc32 -w- -O tutorial.cpp
echo.
echo ** Compiling 'tetris'
echo.
bcc32 -w- -O tetris.cpp
echo.
echo ** Compiling 'wavelet_atrous'
echo.
bcc32 -w- -O wavelet_atrous.cpp
echo.
echo All done, now type 'cd ../../examples' to test the compiled programs.
echo.

