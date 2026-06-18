project('THzCommon', 'cpp', version: '1.0.0', license: 'MIT')

include_dirs = include_directories('include')

sources = files(
	'src/configuration/configuration.cpp',
	'src/configuration/configurationbuilder.cpp',
	'src/configuration/configurationstorage.cpp',
	'src/converter/base64.cpp',
	'src/converter/huffmancoder.cpp',
	'src/converter/huffmancommons.cpp',
	'src/diagnostics/hexview.cpp',
	'src/diagnostics/stopwatch.cpp',
	'src/logging/logging.cpp',
	'src/math/point.cpp',
	'src/math/rectangle.cpp',
	'src/network/address.cpp',
	'src/network/privatecommon.hpp',
	'src/network/socketbase.cpp',
	'src/network/tcpconnection.cpp',
	'src/network/tcpsocket.cpp',
	'src/network/udpsocket.cpp',
	'src/random/ant.cpp',
	'src/utility/bitbuffer.cpp',
	'src/utility/byteorder.cpp',
	'src/utility/range2D.cpp',
	'src/utility/range2DFolding.cpp',
	'src/utility/stringviewhelpers.cpp',
	'src/utility/time.cpp',
)

gsl_proj = subproject('microsoft-gsl')
gsl_dep = gsl_proj.get_variable('microsoft_gsl_dep')

if build_machine.system() == 'windows'
	sources += files('src/network/common_win32.cpp')
	dependencies = [gsl_dep, meson.get_compiler('cpp').find_library('ws2_32')]
else
	sources += files('src/network/common_posix.cpp')
    dependencies = [gsl_dep]
endif

thzcommon_lib = library(
	meson.project_name(),
	sources,
	include_directories: include_dirs,
	dependencies: dependencies,
	override_options: ['cpp_std=c++20'],
)

thzcommon_dep = declare_dependency(include_directories: include_dirs, link_with: thzcommon_lib)

test_sources = files(
	'test/configuration/configuration.cpp',
	'test/configuration/configurationbuilder.cpp',
	'test/configuration/configurationstorage.cpp',
	'test/converter/base64.cpp',
	'test/converter/huffmancommons.cpp',
	'test/logging.cpp',
	'test/math/bilinearInterpolation.cpp',
	'test/math/inrange.cpp',
	'test/math/matrix.cpp',
	'test/math/point.cpp',
	'test/math/rectangle.cpp',
	'test/memory/addresshelper.cpp',
	'test/network/address.cpp',
	'test/network/tcpconnection.cpp',
	'test/network/tcpsocket.cpp',
	'test/network/udpsocket.cpp',
	'test/random/ant.cpp',
	'test/structures/octree.cpp',
	'test/structures/queue.cpp',
	'test/structures/stack.cpp',
	'test/utility/bitbuffer.cpp',
	'test/utility/byteorder.cpp',
	'test/utility/flipBuffer.cpp',
	'test/utility/fstreamhelpers.cpp',
	'test/utility/lineSequencer.cpp',
	'test/utility/range2D.cpp',
	'test/utility/range2DFolding.cpp',
	'test/utility/result.cpp',
	'test/utility/spanhelpers.cpp',
	'test/utility/sptesthelper.cpp',
	'test/utility/sptesthelper.hpp',
	'test/utility/staticPImpl.cpp',
	'test/utility/stringhelpers.cpp',
	'test/utility/stringviewhelpers.cpp',
)

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

test_deps = []
test_deps += dependencies
test_deps += gtest_dep
test_deps += gmock_dep

test_exe = executable(
	'THzCommonTests',
	sources + test_sources,
	include_directories: include_dirs,
	dependencies: test_deps,
	override_options: ['cpp_std=c++20'],
)

test('THzCommonTests', test_exe)