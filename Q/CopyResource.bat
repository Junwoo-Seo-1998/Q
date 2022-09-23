echo "copy data.."
if exist "bin/Debug-windows-x86_64/Q" (
    robocopy "external/bin/" "bin/Debug-windows-x86_64/Q/"
    robocopy "Q/resource/" "bin/Debug-windows-x86_64/Q/resource/" /MIR
)

if exist "bin/Debug-windows-x86_64/Q-Editor" (
    robocopy "external/bin/" "bin/Debug-windows-x86_64/Q-Editor/"
    robocopy "Q/resource/" "bin/Debug-windows-x86_64/Q-Editor/resource/" /MIR
)

if exist "bin/Release-windows-x86_64/Q" (
    robocopy "external/bin/" "bin/Release-windows-x86_64/Q/"
    robocopy "Q/resource/" "bin/Release-windows-x86_64/Q/resource/" /MIR
)

if exist "bin/Release-windows-x86_64/Q-Editor" (
    robocopy "external/bin/" "bin/Release-windows-x86_64/Q-Editor/"
    robocopy "Q/resource/" "bin/Release-windows-x86_64/Q-Editor/resource/" /MIR
)