> This Sowftware work with my bords in this case only SWICHES.
>
> you can use PCA9685 BOARD
>
> V1.0 work with 8 different OE PINs from CPU Board , so you have max 128 PWMs ( more is posible when you groupe diffrent PCA9685s on OE PINs )
> 
+ adr 1-16  use only one servo on first PCA
+ adr 17-32  use only one servo on second PCA
+ etc..
+ adr 1001-1008 use 2 servos on first PCA
+ adr 1011-1018 use 2 servos on second PCA
+ etc..
