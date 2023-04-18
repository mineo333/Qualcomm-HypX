Intro to HypX
===============

HypX is the hypervisor of Samsung phones running the Qualcomm processor. 


Getting the binary
--------------------

Getting the binary is actually fairly simple as it is part of the firmware. The phone being used is an SM-G9860, so we can just use a tool like SamFirm or Frija to get the firmware. Unzipping the firmware and the BL archive, we will find a binary called ``hyp.mbn``. This is the hypervisor binary. Luckily, this binary is an ELF, so we know where it will be loaded. 

Ghidra
-------

