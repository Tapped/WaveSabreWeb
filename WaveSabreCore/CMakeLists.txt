add_library(WaveSabreCore
	include/WaveSabreCore/AllPass.h
	include/WaveSabreCore/AllPassDelay.h
	include/WaveSabreCore/BiquadFilter.h
	include/WaveSabreCore/Cathedral.h
	include/WaveSabreCore/Chamber.h
	include/WaveSabreCore/Comb.h
	include/WaveSabreCore/Crusher.h
	include/WaveSabreCore/DelayBuffer.h
	include/WaveSabreCore/Device.h
	include/WaveSabreCore/Devices.h
	include/WaveSabreCore/Echo.h
	include/WaveSabreCore/Envelope.h
	include/WaveSabreCore/Falcon.h
	include/WaveSabreCore/Helpers.h
	include/WaveSabreCore/Leveller.h
	include/WaveSabreCore/MxcsrFlagGuard.h
	include/WaveSabreCore/ResampleBuffer.h
	include/WaveSabreCore/SamplePlayer.h
	include/WaveSabreCore/Scissor.h
	include/WaveSabreCore/Slaughter.h
	include/WaveSabreCore/Smasher.h
	include/WaveSabreCore/StateVariableFilter.h
	include/WaveSabreCore/SynthDevice.h
	include/WaveSabreCore/Twister.h
  #include/WaveSabreCore/Adultery.h
	#include/WaveSabreCore/GmDls.h
	#include/WaveSabreCore/GsmSample.h
	#include/WaveSabreCore/Specimen.h
	#include/WaveSabreCore/Deprecated/Thunder.h

	src/AllPass.cpp
	src/AllPassDelay.cpp
	src/BiquadFilter.cpp
	src/Cathedral.cpp
	src/Chamber.cpp
	src/Comb.cpp
	src/Crusher.cpp
	src/DelayBuffer.cpp
	src/Device.cpp
	src/Echo.cpp
	src/Envelope.cpp
	src/Falcon.cpp
	src/Helpers.cpp
	src/Leveller.cpp
	src/MxcsrFlagGuard.cpp
	src/ResampleBuffer.cpp
	src/SamplePlayer.cpp
	src/Scissor.cpp
	src/Slaughter.cpp
	src/Smasher.cpp
	src/StateVariableFilter.cpp
	src/SynthDevice.cpp
	src/Twister.cpp
#   src/Main.cpp
  src/NewImpl.cpp

  # src/Adultery.cpp
  # src/GmDls.cpp
	# src/GsmSample.cpp
  # src/Specimen.cpp
  # src/Deprecated/Thunder.cpp
)



target_link_libraries(WaveSabreCore)
target_include_directories(WaveSabreCore PUBLIC include)
