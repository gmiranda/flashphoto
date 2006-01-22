------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------
   ____  _   _  ____     ___  ____  __  __  ___     __    ____  ____  ____    __    ____  _  _ 
  (_  _)( )_( )( ___)   / __)(_  _)(  \/  )/ __)   (  )  (_  _)(  _ \(  _ \  /__\  (  _ \( \/ )
    )(   ) _ (  )__)   ( (__  _)(_  )    (( (_-.    )(__  _)(_  ) _ < )   / /(__)\  )   / \  / 
   (__) (_) (_)(____)   \___)(____)(_/\/\_)\___/   (____)(____)(____/(_)\_)(__)(__)(_)\_) (__) 


                               The C++ Template Image Processing Library

                                   ( http://cimg.sourceforge.net )

                                               v.1.0.9

------------------------------------------------------------------------------------------------

# Summary
#---------

  The CImg Library is an open-source C++ toolkit for image processing.
  It consists in a single header file 'CImg.h' providing a set of C++ classes that
  can be used in your own sources, to load/save, process and display images. Very portable 
  (Unix/X11,Windows, MacOS X, FreeBSD,..), efficient, simple to use, it's a pleasant toolkit
  for coding image processing stuffs in C++.
  
# Author
#--------

  David Tschumperle  ( http://www.greyc.ensicaen.fr/~dtschump/ )

# Institution
#-------------

 The CImg Library project started during the year 2000, at the INRIA-Sophia Antipolis/France
 (http://www-sop.inria.fr) when I did my PhD thesis.
 Since October 2004, it is maintained and developed in the Image group of the GREYC Lab (CNRS, UMR 6072),
 in Caen/France (http://www.greyc.ensicaen.fr/EquipeImage).

# License
#---------

 The CImg Library is distributed under the CeCiLL license (http://www.cecill.info/index.en.html).
 The CeCiLL license is a free-software license, created under the supervision of the three biggest 
 research institutions on computer sciences in France :

   - CNRS  (http://www.cnrs.fr)
   - CEA   (http://www.cea.fr)
   - INRIA (http://www.inria.fr)

 The CeCiLL license is compatible with the GNU GPL : you can legally redistribute CImg-based programs in GPL.
 You have to RESPECT this license. Particularly, all distributed programs using parts of the CImg Library MUST
 be open-source. Please carefully read the license terms.
 If you are interested to distribute programs using CImg source codes as a closed-source product,
 you are invited to contact David Tschumperle (mail on his web page) to negociate a specific license.

# Package structure :
#--------------------

  The directory CImg/ is organized as follows :

  - CImg.h         : The single (header) file of the library itself.
  - CHANGES.txt    : A list of changes between successive CImg versions, since CImg 1.0.5.
  - LICENSE.txt    : A copy of the CeCiLL license which applies to the CImg Library (English version).
  - LICENSE_FR.txt : A copy of the CeCiLL license which applies to the CImg Library (French version).
  - README.txt     : This file.

  - examples/      : Directory containing a lot of example programs performing various stuffs,
                     based on the CImg library.
    
   * CImg_test.cpp                : A multi-part demo of the CImg capabilities
                                    (should be tested first to get an overview of the CImg capabilities).
   * fade_images.cpp              : Perform an oriented fading between two images.
   * greysctoration.cpp           : An implementation of the greycstoration algorithm for image denoising,
				    inpainting and interpolation.
				    (see http://www.greyc.ensicaen.fr/~dtschump/greycstoration)
   * hough_transform.cpp          : An implementation of the Hough Transform to detect lines
                                    in images.
   * inrcast.cpp                  : Tool used to convert 2D/3D images with command line arguments.
   * odykill.cpp                  : A simple shoot-em-up game, featuring the team of the Odyssee Lab.
   * mcf_levelsets.cpp            : Evolution of a 2D curve, using level sets.
   * image_registration.cpp       : Register two images and do an automatic morphing between them.
   * image2ascii.cpp              : A correlation-based converter of images to ASCII-art.
   * pde_heatflow2D.cpp           : An implementation of the Heat Flow PDE applied on a 2D image.
   * pde_TschumperleDeriche2D.cpp : An implementation of the restoration/inpainting PDE, as described 
                                    in Tschumperle-Deriche:CVPR'03 (monoscale).
   * pde_TschumperleDeriche3D.cpp : Same as 'pde_TschumperleDeriche2D.cpp' for 3D volumes of data.
   * pslider.cpp                  : A tool to generate and view presentations from Postscript files.
   * render3d.cpp                 : An example of 3D object rendering using CImg drawing functions.
   * tetris.cpp                   : An implementation of the well-known tetris game in few lines.
   * tutorial.cpp                 : A very simple code to get started with the CImg library.
   * wavelet_atrous.cpp           : An exemple of wavelet transform.

  - documentation/ : Directory containing a copy of the CImg web page in html format. To generate the 
                     reference documentation, you have to use the tool 'doxygen' 
                     (see http://www.doxygen.org) on the file 'CImg.doxygen'. You can also access the
                     documentation directly from the library web page : http://cimg.sourceforge.net.
                     
  - compilation/   : Directory containing several sub-directories that are intended to ease the 
                     compilation of the examples, with various C++ compilers on Unix and Windows.

# Getting started
#-----------------
 
  First, you should try to compile the different examples provided in the 'examples/'
  directory. Look at the 'compilation/' directory to ease this compilation step
  on different plateforms. Then, you can look at the documentation 'documentation/head.html'
  and the complete reference 'documentation/reference/index.html' to learn more about CImg
  functions and classes. In case of problems, you can go to the 'Forum' section of the
  CImg web page and ask for help.

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------