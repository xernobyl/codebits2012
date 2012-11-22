target = 'boom'
compiler = 'g++'
exceptions = ['audio.c', 'stb_vorbis.cpp', 'Renderer.cpp', 'Cube.cpp']
extensions = ['.cpp', '.c']
compilerflags = ['-Wall', '-std=c++03', '-mfp16-format=ieee', '-O3', '-ftree-vectorize', '-fPIC', '-DSTANDALONE', '-D__STDC_CONSTANT_MACROS', '-D__STDC_LIMIT_MACROS',
	'-DTARGET_POSIX', '-D_LINUX', '-DPIC', '-D_REENTRANT', '-D_LARGEFILE64_SOURCE', '-D_FILE_OFFSET_BITS=64', '-DHAVE_LIBOPENMAX=2', '-DOMX',
	'-DOMX_SKIP64BIT', '-DUSE_EXTERNAL_OMX', '-DHAVE_LIBBCM_HOST', '-DUSE_EXTERNAL_LIBBCM_HOST', '-DUSE_VCHIQ_ARM', '-U_FORTIFY_SOURCE']
includes = ['-I/opt/vc/include/', '-I/opt/vc/include/interface/vcos/pthreads']
libraries = ['-L/opt/vc/lib', '-lGLESv2', '-lEGL', '-lrt', '-lbcm_host', '-lvchiq_arm', '-lvcos']


# # #
#   #
# # #


from subprocess import call
import os

sourcefiles = []
objectfiles = []


def FindSourceFiles():
	for root, subFolders, files in os.walk("."):
		for file in files:
			for extension in extensions:
				if file.endswith(extension) and file not in exceptions:
					sourcefiles.append(os.path.join(root,file))


def Clean():
	
	ret = call(['rm *.o'])
	if ret == 0:
		print("Successfully removed .o files.")
	else:
		print("Could not remove files.")


def Compile():
	for sourcefile in sourcefiles:
		objectfile = os.path.splitext(sourcefile)[0] + '.o'
		try:
			time = os.path.getmtime(objectfile)
		except:
			recompile = True
		else:
			recompile = time < os.path.getmtime(sourcefile)
		finally:
			if recompile:
				objectfiles.append(objectfile)
				print('Recompiling "' + sourcefile + '".')
				args = [compiler]
				args += compilerflags
				args += includes
				args.append('-c')
				args.append(sourcefile)
				args.append('-o')
				args.append(objectfile)

				ret = call(args)
				if ret != 0:
					return False
	return True


def Link():
	if len(objectfiles) == 0 and os.path.exists(target):
		print('No files modified.')
	else:
		print('Linking...')
		args = [compiler]
		args.append('-o')
		args.append(target)
		args.append('-Wl,--whole-archive')
		args += objectfiles
		args += libraries
		args.append('-Wl,--no-whole-archive')
		args.append('-rdynamic')

		ret = call(args)
		if ret != 0:
			return False
		else:
			return True


FindSourceFiles()
if Compile():
	Link()
