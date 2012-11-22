OBJS=main.o GLWindow.o Quad.o Shaders.o Resources.o Camera.o FlatCamera.o Particles.o Text.o Font.o stb_image.o Model.o Background.o BGCircles.o BGPlanes.o BGTunnel.o Slide.o TextSlide.o
BIN=main.bin

CFLAGS+= -Wall -std=c++03 -mfp16-format=ieee -O3 -ftree-vectorize -fPIC
CFLAGS+=-DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM
CFLAGS+=-U_FORTIFY_SOURCE

LDFLAGS+=-L/opt/vc/lib -lGLESv2 -lEGL -lrt -lbcm_host -lvchiq_arm -lvcos
INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads

all: $(BIN) $(LIB)

%.o: %.cpp
	#@rm -f $@ 
	g++ $(CFLAGS) $(INCLUDES) -c $< -o $@

%.bin: $(OBJS)
	g++ -o $@ -Wl,--whole-archive $(OBJS) $(LDFLAGS) -Wl,--no-whole-archive -rdynamic

%.a: $(OBJS)
	$(AR) r $@ $^

clean:
	for i in $(OBJS); do (if test -e "$$i"; then ( rm $$i ); fi ); done
	@rm -f $(BIN) $(LIB)
