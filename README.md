###### simple bundle kernel + usermode to easy dumping process modules

###### dependencies:
> ksocket: https://github.com/wbenny/KSOCKET/ <br>
> FindWDK: https://github.com/SergiusTheBest/FindWDK/ <br>

###### inspired by:
> https://github.com/EquiFox/KsDumper <br>
> https://github.com/justvmexit/dumpr <br>
<br>

###### todo:
> hide the threads on driver <br>
> refactory driver code <br>
> add more error handlers on driver <br>
> report errors from driver to usermode <br>
> rewrite ksocket to more modern decision <br>

###### it is recommended to use with kdmapper (https://github.com/TheCruZ/kdmapper)
###### it is not recommended to use with an active anti-cheats (EAC, BE, etc...) (the dump will be successful, but it is possible to block/tag the account (BSOD is also possible, the success was checked only on RAGE:MP and BattleBit))
