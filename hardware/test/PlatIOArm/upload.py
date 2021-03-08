Import("env")

# please keep $SOURCE variable, it will be replaced with a path to firmware

# Generic
env.Replace(
    ARG1="-pn 4",
    ARG2="-br 115200",
    ARG3="-v",
    ARG4="-fn",
    ARG5="-a 08000000",
    ARG6="-all",
    # UPLOADCMD="STMFlashLoader.exe -c -$ARG1 -$ARG2 -i STM32F1_High-density_256K -d --v --fn $SOURCE"
    UPLOADCMD="STMFlashLoader.exe -c -$ARG1 -$ARG2 -i STM32F1_High-density_256K -e -$ARG6 -d -$ARG3 -$ARG4 $SOURCE"
)

# In-line command with arguments
# env.Replace(
#     UPLOADCMD="executable -arg1 -arg2 $SOURCE"
# )

# Python callback
# def on_upload(source, target, env):
#     print(source, target)
#     firmware_path = str(source[0])
#     # do something
#     env.Execute("executable arg1 arg2")

# env.Replace(UPLOADCMD=on_upload)