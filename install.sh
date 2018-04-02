#!/bin/bash

# Custom install script by AidoP

while true; do
    read -p "You are not using Solus. This may cause problems. Continue? (y/n)\

> " yn
    case $yn in
        [Yy]* ) break;;
        [Nn]* ) echo Aborted; exit;;
        * ) echo Aborted; exit;;
    esac
done

echo Installing...