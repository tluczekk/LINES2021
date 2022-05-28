#!/bin/bash
(
cd Utility
./build.sh
./copy_to_admin.sh
cd ..
)

(
cd Admin
./user_img/dual_boot.sh
./build.sh
cd ..
)
