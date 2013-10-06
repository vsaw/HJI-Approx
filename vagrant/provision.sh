#! /bin/bash

# Fail on error
set -e

sudo apt-get update --yes

# Needed to install vim plugins
sudo apt-get install --yes unzip exuberant-ctags

# TODO: Set up vim plugins
sudo apt-get install --yes vim
# Taglist plugin
if [ -f ~/.vim/plugin/taglist.vim ];
then
	echo "vim taglist already installed"
else
	wget -O /tmp/taglist_46.zip  http://garr.dl.sourceforge.net/project/vim-taglist/vim-taglist/4.6/taglist_46.zip
	mkdir -p ~/.vim
	unzip -u /tmp/taglist_46.zip -d ~/.vim
	echo "filetype on" >> ~/.vimrc
fi

# Install and run node.js
# The jasmine-node will not finish but continue to wath and test files in node.
sudo apt-get install --yes npm
sudo npm install -g jasmine-node
jasmine-node --autotest /vagrant/html/scripts/spec --watch /vagrant/html/scripts/*.js
