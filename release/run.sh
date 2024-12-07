#!/bin/bash

# Get current DS (Desktop Enviroment)
desktop_env=$(echo $XDG_CURRENT_DESKTOP | awk '{print tolower($0)}')

case "$desktop_env" in
    *gnome*)
        terminal="gnome-terminal"
        ;;
    *kde|*plasma*)
        terminal="konsole"
        ;;
    *xfce*)
        terminal="xfce4-terminal"
        ;;
    *)
        echo "Unknown desktop env. Open current directory in bash and execute /'./bin/app/' to run program."
        exit 1
        ;;
esac


$terminal -- ./bin/app