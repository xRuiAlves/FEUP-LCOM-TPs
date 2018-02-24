# Move the configuration file to the system folder
cp conf/SnakeyNix /etc/system.conf.d

# Copy the resources to a neutral-location to be easily accessed
mkdir /home/SnakeyNix
cp -vr resources/*.bmp /home/SnakeyNix

# Add regular user permission to 'compile' and 'play' scripts
chmod 777 compile.sh
chmod 777 play.sh

echo -e "\n Installation finished.\n Write 'sh compile.sh' to compile the code and 'sh play.sh' to run the game.\n"
