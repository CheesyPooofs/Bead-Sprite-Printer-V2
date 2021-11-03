# Bead-Sprite-Printer-V2
Contains firmware and software used for the bead sprite printer v2 robotic system

bead_fuser_arduino - this folder contains the Arduino file that controls realys that allow mains power to heat up 4 heat beds.
  heat beds used - https://www.amazon.com/gp/product/B07FB1TZ25/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
   relays used - https://www.amazon.com/uxcell-ASH-25DA-3-32VDC-480VAC-Authorized/dp/B01LXLEBHQ/ref=pd_bxgy_img_1/138-7212191-5019650?pd_rd_w=kCtmR&pf_rd_p=c64372fa-c41c-422e-990d-9e034f73989b&pf_rd_r=K6ANF0ADSQT6FQHT16DK&pd_rd_r=a30d7c6e-c4ac-4231-ae87-8970aa6dcb62&pd_rd_wg=AcxAN&pd_rd_i=B01LXLEBHQ&psc=1
   
bead_picker_placer_arduino - this folder contains the arduino files that controls the bead pick up mechanism (multiple servos, laser, light sensors, solinoid) as well as the bead placer mechanism (2 servos, LED, photodiodes) the Arduino also communicates with the marlin controller that controls the cnc machine
  servos used - https://www.amazon.com/gp/product/B07WYQ9P3F/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
 
Custom Marlin firmware - Based off the Marlin firmware used for 3d printers, this custom version has additional g code commands that allows more customized features with the machine as well as communication to the helper arduino

beadv2_converter - this is the main python project that converts images to pixellated images matching the color pallet of the machine. It also converts the image to g code for the machine to run

Electrical - Doesnt show the electical or wiring changes made but contains schematic for marlin as well as some close up photos of wiring (sorry!)
 

  
