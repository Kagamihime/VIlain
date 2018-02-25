#!/bin/bash

if [[ "$1" = "install" ]]; then
    python3 -m venv coala-venv
    source coala-venv/bin/activate

    git clone https://github.com/coala/coala
    git clone https://github.com/coala/coala-bears
    pip3 install coala/
    pip3 install coala-bears/
    rm -rf coala/
    rm -rf coala-bears/

    deactivate

elif [[ "$1" = "run" ]]; then
    source coala-venv/bin/activate

    coala

    deactivate

elif [[ "$1" = "clean" ]]; then
    if [[ -d "coala" ]]; then
        rm -rf coala
    fi

    if [[ -d "coala-bears" ]]; then
        rm -rf coala-bears
    fi

    if [[ -d "coala-venv" ]]; then
        rm -rf coala-venv
    fi

else
    echo "Help:"
    echo "install    - Install coala in the project's root"
    echo "run        - Run coala if it is already installed"
    echo "clean      - Remove coala and the python environment from the project's root"
fi
