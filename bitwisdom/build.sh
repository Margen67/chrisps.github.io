/opt/cheerp/bin/clang++ -fstrict-aliasing -fstrict-enums -std=gnu++1z -cheerp-preexecute -cheerp-no-math-imul -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -fmerge-all-constants -fno-stack-protector -fomit-frame-pointer -Os -target cheerp bitwisdom.cpp -o bitwisdom.js
~/jarfiles/closure.jar --compilation_level ADVANCED --assume_function_wrapper --third_party --language_out ECMASCRIPT5 --js bitwisdom.js --js_output_file bitwisdom_compiled.js
