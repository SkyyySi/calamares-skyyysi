#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess
import libcalamares
from pathlib import Path

root_mount_point = libcalamares.globalstorage.value("rootMountPoint")
    
def run():
    """
    Installing packages. This may take a while.
    """

    PACSTRAP = "sudo /usr/bin/pacstrap_calamares"
    PACKAGES = "amd-ucode base base-devel breeze-icons dracut efibootmgr firefox firefox-ublock-origin firefox-dark-reader git grml-zsh-config grub gtk2 gtkmm3 intel-ucode kvantum-qt5 pcmanfm linux-firmware linux-lts-zen linux-lts-headers nano networkmanager nitrogen nm-applet nordic-kde-git nordic-polar-theme-git nordic-theme-git open-vm-tools os-prober picom-ibhagwan-git plymouth termite vim vlc xf86-input-elographics xf86-input-libinput xf86-input-vmmouse xf86-video-amdgpu xf86-video-ati xf86-video-dummy xf86-video-fbdev xf86-video-intel xf86-video-nouveau xf86-video-openchrome xf86-video-sisusb xf86-video-vesa xf86-video-vmware xf86-video-voodoo xf86-video-qxl yay zsh"

    subprocess.call(PACSTRAP.split(' ') + [root_mount_point] + PACKAGES.split(' '))
