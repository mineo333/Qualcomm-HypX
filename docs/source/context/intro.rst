Intro to AArch64 Architecture
===============================

The purpose of this document is to cover the basics of AArch64 (Cortex-A) architecture. This document is mainly intended for those with experience in system programming.

For ease of use, a lot of parallels will be drawn with x86 structures. 

Exception Levels
------------------

Exception Levels are the equivalent of privliege levels or "rings" in x86. There are 4 exception levels:

* EL0 - Userspace
* El1 - Kernel
* EL2 - Hypervisor
* EL3 - Secure Monitor

EL0 and EL1 work similar to how they do in x86. EL2 is where the hypervisor operates as opposed to the VMX/VMCS stuff that happens in x86. EL3 is where the secure monitor operates. The Secure Monitor serves as an entry point to the secure world. 

In this project, we are primarily working at the EL2 level with some calls being made to EL3. 

System Registers
------------------

There are a variety of system registers used in AArch64. These system registers are written to using the ``msr`` instruction and read from using the ``mrs`` instruction. These registers are very similar to model specific registers in x86.


Transitioning Between Exception Levels
---------------------------------------

There are a variety of ways that one can transition between exception levels. The first way is via a direct call - similar to a system call.

To transition to EL1, we use the ``svc`` instruction. This is how syscalls are performed.

To transition to EL2, we use the ``hvc`` instruction. This is usually to faciliate hypercalls. This instruction may only be called from EL1 or above. 

To transition to El3, we use the ``smc`` instruction. This is used to faciliate transitions to the secure world or call trustzone applications. This instruction may only be called from EL1 or above. 

We may also transition on faults such as MMU faults, instruction alignment faults, etc. A full list for why we may transition to each Exception Level can be found below specifically in the EC table:

* `ESR_EL1 <https://developer.arm.com/documentation/ddi0601/2020-12/AArch64-Registers/ESR-EL1--Exception-Syndrome-Register--EL1->`_
* `ESR_EL2 <https://developer.arm.com/documentation/ddi0601/2022-03/AArch64-Registers/ESR-EL2--Exception-Syndrome-Register--EL2->`_
* `ESR_EL3 <https://developer.arm.com/documentation/ddi0595/2021-06/AArch64-Registers/ESR-EL3--Exception-Syndrome-Register--EL3->`_

In addition to direct calls, we may also transition via an IRQ, FIQ, or an System Error (SError). The exception level that an IRQ or FIQ are taken to depends on whether or not certain configurations in ``HCR_EL2`` and ``SCR_EL3`` are enabled. 

We will mainly be focusing on two cases: direct calls and MMU faults in the Qualcomm hypervisor

What happens when we transition states? When we transition states there are 3 system registers involved: ``SPSR_ELx``, ``ELR_ELx``, and ``ESR_ELx`` or the saved process state register, the exception link register, and the exception syndrome register. 

Suppose we transiton to EL2, the ``PSTATE`` of the process before the transition is placed into ``SPSR_EL2``, the return address (``PC+4``) is placed in ``ELR_EL2``, and the reason why we go to EL2 is placed in ``ESR_EL2``. When we return using the ``eret`` instruction, we restore ``SPSR_EL2`` to ``PSTATE`` and ``ELR_EL2`` to PC. 


Another important question is where do we transition? That answer is dependent on the base address in the register ``VBAR_ELx``



