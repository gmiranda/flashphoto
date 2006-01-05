@echo off
echo.
echo Compiling CImg with Digital Mars Compiler (DMC)
echo.

cd ../../examples
echo.
echo ** Compiling 'CImg_test'
echo.
dmc -Ae -O CImg_test.cpp gdi32.lib
echo.
echo ** Compiling 'fade_images'
echo.
dmc -Ae -O fade_images.cpp gdi32.lib
echo.
echo ** Compiling 'greycstoration'
echo.
dmc -Ae -O greycstoration.cpp gdi32.lib
echo.
echo ** Compiling 'hough_transform'
echo.
dmc -Ae -O hough_transform.cpp gdi32.lib
echo.
echo ** Compiling 'inrcast'
echo.
dmc -Ae -O inrcast.cpp gdi32.lib
echo.
echo ** Compiling 'odykill'
echo.
dmc -Ae -O odykill.cpp gdi32.lib
echo.
echo ** Compiling 'mcf_levelsets'
echo.
dmc -Ae -O mcf_levelsets.cpp gdi32.lib
echo.
echo ** Compiling 'image_registration'
echo.
dmc -Ae -O image_registration.cpp gdi32.lib
echo.
echo ** Compiling 'image2ascii'
echo.
dmc -Ae -O image2ascii.cpp gdi32.lib
echo.
echo ** Compiling 'pde_heatflow2D'
echo.
dmc -Ae -O pde_heatflow2D.cpp gdi32.lib
echo.
echo ** Compiling 'pde_TschumperleDeriche2D'
echo.
dmc -Ae -O pde_TschumperleDeriche2D.cpp gdi32.lib
echo.
echo ** Compiling 'pslider'
echo.
dmc -Ae -O pslider.cpp gdi32.lib
echo.
echo ** Compiling 'render3d'
echo.
dmc -Ae -O render3d.cpp gdi32.lib
echo.
echo ** Compiling 'tutorial'
echo.
dmc -Ae -O tutorial.cpp gdi32.lib
echo.
echo ** Compiling 'tetris'
echo.
dmc -Ae -O tetris.cpp gdi32.lib
echo.
echo ** Compiling 'wavelet_atrous'
echo.
dmc -Ae -O wavelet_atrous.cpp gdi32.lib
echo.
echo All done, now type 'cd ../../examples' to test the compiled programs.
echo.

