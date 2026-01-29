#! /bin/sh

getModel(){
  declare -A models=(
    ["teapot"]="https://groups.csail.mit.edu/graphics/classes/6.837/F03/models/teapot.obj"
    ["bunny"]="https://raw.githubusercontent.com/alecjacobson/common-3d-test-models/master/data/stanford-bunny.obj"
    ["cow"]="https://groups.csail.mit.edu/graphics/classes/6.837/F03/models/cow-nonormals.obj"
    ["pumpkin"]="https://groups.csail.mit.edu/graphics/classes/6.837/F03/models/pumpkin_tall_10k.obj"
    ["teddy"]="https://groups.csail.mit.edu/graphics/classes/6.837/F03/models/teddy.obj"
  )
  choice=$1
  location=$2
  if [ $# -ne 2 ]; then
    echo "please select a model to view:"
    for k in "${!models[@]}"; do
      echo "$k"
    done
    read -p "Choice: " choice
    location="model.obj"
  fi
  url="${models[$choice]}"
  if [ -z "$url" ]; then
    echo "bad choice"
    exit 1
  fi
  if wget -q $url -O "$location"; then
    echo "$choice model downloaded sucessfully"
  else
    echo "failed to download $choice model"
    exit 1
  fi
}

gifFromPpm(){
  if [ $# -ne 1 ]; then
    echo "usage: gif <folder containing ppm files>"
    exit 1
  fi
  if [ ! -d "$1" ]; then
    echo "folder does not exist"
    exit 1
  fi
  echo "creating gif"
  convert -delay 10 -loop 0 $(ls "$1"/*.ppm) output.gif
  echo "gif created sucessfully"
}


if [ ! -f "model.obj" ]; then 
  getModel;
fi
echo "Enter 1 to download a model"
echo "Enter 2 to launch a terminal visualisation"
echo "Enter 3 to export a render to a gif"
echo "Enter 4 to lauch a interactive x11 window"
echo "Enter 5 to lauch a window with a sin wave"
read -p "Choice:" choice

case $choice in
  0)make link FILENAME=examples/debug.c IMEJ_INPUT=true || { echo "compilation error" && exit 1; }
    read -n1 -r -p "press any key to start program"
    ./imej ;;
  1) getModel;;
  2) make link FILENAME=examples/term.c IMEJ_DISPLAY=term || { echo "compilation error" && exit 1; }
    ./imej ;;
  3) make link FILENAME=examples/ppmExample.c IMEJ_DISPLAY=ppm || { echo "compilation error" && exit 1; }
    mkdir -p frames
    ./imej || echo "failed"
    gifFromPpm "frames/";;
  4)make link FILENAME=examples/x11Demo.c IMEJ_DISPLAY=x11 IMEJ_INPUT=true || { echo "compilation error" && exit 1; }
    ./imej ;;
  5)make link FILENAME=examples/chart.c IMEJ_DISPLAY=x11 IMEJ_INPUT=true || { echo "compilation error" && exit 1; }
    read -n1 -r -p "press any key to start program"
    ./imej ;;
esac

# echo "Imej can be compiled using \"make compile\""
# echo "By default, a perspective projection is chosen and displayed in an x11 window"
# echo "To switch projections manually, compile using \"make imej IMEJ_PROJECTION=<orthogonal|perspective>\""
# echo "To switch the output type, compile using \"make imej IMEJ_DISPLAY=<ppm|x11>\""
# echo "Example applications include"
# echo "1) run a simulations, exporting frames to ppm images and then compile to a gif"
# echo "2) data visualisation"
# echo "3) x11 graphical window using an orthogonal projection, simulation reacts to user input"
# echo "4) x11 graphical window using a perspective projection, simulation reacts to user input"
# read -p "Choice:" choice

# case $choice in
#   0) echo "debug script"
#     if [ ! -f "teapot.obj" ] || [ ! -f "bunny.obj" ];then
#       getModel "teapot" teapot.obj
#       getModel "bunny" bunny.obj
#     fi
#     make demo FILENAME=examples/debug.c IMEJ_DISPLAY=x11 IMEJ_PROJECTION=perspective || { echo "compilation error" && exit 1; }
#     read -n1 -r -p "press any key to start program"
#     ./imej ;;
#   1) echo "ppm gif render"
#     getModel
#     make demo FILENAME=examples/ppmExample.c IMEJ_DISPLAY=ppm IMEJ_PROJECTION=perspective || { echo "compilation error" && exit 1; }
#     mkdir -p frames
#     ./imej || echo "failed"
#     gifFromPpm "frames/";;
#   2) echo "data visualisation"
#     python assets/vegSurvey.py
#     make demo FILENAME=examples/x11Interactive.c IMEJ_DISPLAY=x11 IMEJ_PROJECTION=perspective || { echo "compilation error" && exit 1; }
#     ./imej;;
#   3) echo "interactive orthogonal"
#     getModel "teapot" teapot.obj
#     getModel "bunny" bunny.obj
#     make demo FILENAME=examples/x11Interactive.c IMEJ_DISPLAY=x11 IMEJ_PROJECTION=orthogonal || { echo "compilation error" && exit 1; }
#     ./imej;;
#   4) echo "interactive window"
#     getModel "teapot" teapot.obj 
#     getModel "bunny" bunny.obj
#     make demo FILENAME=examples/x11Interactive.c IMEJ_DISPLAY=x11 IMEJ_PROJECTION=perspective || { echo "compilation error" && exit 1; }
#     read -n1 -r -p "press any key to start program"
#     ./imej ;;
#   *) echo "bad choice";;
# esac
