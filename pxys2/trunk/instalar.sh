#! /bin/sh

echo "IRC-Hispano Proxy Scanner"
echo "Basado en el PXYS 2.1"
echo ""
echo "Instalacion del proxy scanner..."
echo ""

if [ x"$1" = x ]
then
        echo "Uso $0 <prefijo ruta>"
        exit 1
fi

cd libpeak
./configure --prefix=$1
make
make check
make install
cd ..
export PATH=$PATH:$1
./configure -C --prefix=$1 --with-maxnicklen=15 
make
make install

echo ""
echo ""
echo "Fin de la instalacion"
exit 0

