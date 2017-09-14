@SET DEFS=-DOPENCV2_DIR:PATH=D:/CODE/OPENCV/opencv_2_4_13_build_vc12x64/install

@ECHO DEFS = %DEFS%

cmake ../hgtrack -G "Visual Studio 12 2013 Win64" %DEFS%
@pause