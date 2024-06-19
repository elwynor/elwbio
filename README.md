<pre>   
     __   _          __        __  __       
    / /  (_)__  ____/ /  __ __/ /_/ /  __ _ 
   / _ \/ / _ \/ __/ _ \/ // / __/ _ \/  ' \
  /_.__/_/\___/_/ /_//_/\_, /\__/_//_/_/_/_/
                       /___/                
                                        
                version 2.0
  Copyright (c) 2004-2024 Elwynor Technologies
</pre>          

## WHAT IS BIORHYTHM?

 Biorhythm charts your daily biorhythmic behavioral patterns over a thirty
 day period in an easy to read ANSI output.
 
## GAMEPLAY
 
 Simply enter your birthday (Month, Day, Year) and watch the chart.  
 
 The median line (0) is your "Critical" point and is when you are most 
 prone to accidents or bad luck.  During the "High" periods (+), you are 
 the most energetic and will have better luck than usual.  "Low" periods 
 (-) are times when you have no ambition, and will experience the worst 
 luck.

## GRAPH SYMBOLS
<pre>
 I ... Median Line        (0)
 P ... Physical Cycle     (23-day)
 S ... Sensitivity Cycle  (28-day)
 C ... Cognitive Cycle    (33-day)
</pre>

## INSTALLATION AND ACTIVATION
 
 Simply unzip the Biorhythm archive to your BBS server directory, add the 
 nodule to your menu, configure the MSG file to your liking, and start the BBS!  
 It's that easy! 

## GAME HISTORY
 
 Biorhythm was originally developed in February 1988 by Don Arnel of 
 Logicom (LOG). Logicom closed its doors in 1997, selling the rights 
 to its modules to various other ISVs. Biorhythm was sold to AdventureComm, 
 which was sold to Elwynor Technologies in 2004.
  
 Rick Hadsall of Elwynor Technologies upconverted the module to Worldgroup
 3.2 in 2005, and it was made free software in 2024.

 The module was converted to Major BBS V10 in June 2024.

## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for more 
 details.

## CONTRIBUTIONS

 We welcome contributions from the community. By contributing, you agree to 
 the terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be 
 licensed under the AGPL v3, assign all right, title, and interest, including all copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and you must include the additional terms from the LICENSE file in your project's LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWBIO" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWAFK.ZIP except the Release-level DLL file. When unzipped in a 
 Worldgroup 3.2 or Major BBS V10 installation folder, it "installs" the module.

