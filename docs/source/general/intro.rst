Intro to HypX
===============

HypX is the hypervisor of Samsung phones running the Qualcomm processor. 


What is HypX and what does it do?
----------------------------------

Every Qualcomm Samsung phone has the standard Android Kernel + Android Userspace. However, to introduce more security, Samsung has introduced a defense-in-depth strategy. The first layer of this defense-in-depth strategy past the kernel is the hypervisor known as HypX. HypX is not a traditional hypervisor. Instead it is an ultra-thin hypervisor that acts as an entity above the kernel and serves to provide security within the kernel among other unknown functionality. The main way this security is provided is via the uH interface. 

The uH interface is the kernel's interface to the hypervisor. The kernel-side of the uH interface is public because Samsung is required to publish their kernel source by GPL. The uH interface is mainly defined by uh.h in the kernel which has the following code:

.. code-block:: C
    :caption: uh.h

    #ifndef __UH_H__
    #define __UH_H__

    #ifndef __ASSEMBLY__

    /* For uH Command */
    #define	APP_INIT	0
    #define	APP_SAMPLE	1
    #define APP_RKP		2
    #define APP_CFP		2 //Will be changed.
    #define APP_HDM		6

    #define UH_APP_INIT		UH_APPID(APP_INIT)
    #define UH_APP_SAMPLE		UH_APPID(APP_SAMPLE)
    #define UH_APP_RKP		UH_APPID(APP_RKP)
    #define UH_APP_CFP		UH_APPID(APP_CFP)
    #define UH_APP_HDM		UH_APPID(APP_HDM)

    #define UH_PREFIX  UL(0xc300c000)
    #define UH_APPID(APP_ID)  ((UL(APP_ID) & UL(0xFF)) | UH_PREFIX)

    /* For uH Memory */
    #define UH_NUM_MEM		0x00

    #define 		0xB0200000
    #define UH_LOG_SIZE		0x40000

    int uh_call(u64 app_id, u64 command, u64 arg0, u64 arg1, u64 arg2, u64 arg3);

    struct test_case_struct {
        int (* fn)(void); //test case func
        char * describe;
    };

    #endif //__ASSEMBLY__
    #endif //__UH_H__

Let's break this down.

The first set of macros are our apps. The hypervisor plays host to a series of APPs. The main ones that we care about are ``APP_RKP`` and ``APP_HDM``. ``APP_RKP`` is known as realtime kernel protection (RKP). RKP is further split into two seperate operations: normal RKP and KDP or Kernel Data Protection. KDP is used to protect structures like slabs. ``APP_HDM`` is the hypervisor device manager (HDM).

The second set of macros are taking those apps and generating the appropriate APP_ID. 

Keep ``UH_PREFIX`` in mind, as we will use that later to identify the ``uh_call`` code. 

In order to access these apps, we need to use ``uh_call`` which makes a call to the hypervisor. ``uh_call`` has the following implementation:

.. code-block::
    :caption: uh_call

    stp x1, x0, [sp, #-16]!
    stp x3, x2, [sp, #-16]!
    stp x5, x4, [sp, #-16]!
    stp x7, x6, [sp, #-16]!
    stp x9, x8, [sp, #-16]!
    stp x11, x10, [sp, #-16]!
    stp x13, x12, [sp, #-16]!
    stp x15, x14, [sp, #-16]!
    #ifdef CONFIG_CFP_ROPP
    stp xzr, x16, [sp, #-16]!
    #else
    stp x17, x16, [sp, #-16]!
    #endif
    stp x19, x18, [sp, #-16]!
    stp x21, x20, [sp, #-16]!
    stp x23, x22, [sp, #-16]!
    stp x25, x24, [sp, #-16]!
    stp x27, x26, [sp, #-16]!
    stp x29, x28, [sp, #-16]!
    stp xzr, x30, [sp, #-16]!

    back:
    smc	#0x0 ;hypervisor call
    cmp	x0, #0x1
    b.eq	back

    ldp xzr, x30, [sp], #16
    ldp x29, x28, [sp], #16
    ldp x27, x26, [sp], #16
    ldp x25, x24, [sp], #16
    ldp x23, x22, [sp], #16
    ldp x21, x20, [sp], #16
    ldp x19, x18, [sp], #16
    #ifdef CONFIG_CFP_ROPP
    ldp xzr, x16, [sp], #16
    #else
    ldp x17, x16, [sp], #16
    #endif
    ldp x15, x14, [sp], #16
    ldp x13, x12, [sp], #16
    ldp x11, x10, [sp], #16
    ldp x9, x8, [sp], #16
    ldp x7, x6, [sp], #16
    ldp x5, x4, [sp], #16
    ldp x3, x2, [sp], #16
    ldp x1, x0, [sp], #16


    ret

The idea behind this code is pretty simple: save all the GP registers, make the call, and, upon return, restore all the GP registers. However, notice how we are making an ``smc`` instead of an ``hvc``. The reason for this is that TSC bit in hcr_el2 is enabled. So, any smc instruction will actually trap to EL2 not EL3. 

``uh_call`` takes in a few parameters. The first parameter is the app ID. This is the app that will be called. The second is the command ID. This is the operation being requested from the app. Unfortuantely, the command ID is only really documented for RKP. We can use context clues to determine a few of the KDP ones, but HDM is almost entirely undocumented. We will discuss these command IDs in the each app's page. 

The fundamental goal of this reversing is to understand what happens when ``smc`` is called.

Getting the Binary, Basic Reversing
-------------------------------------

Getting the binary for the hypervisor is actually quite easy as it's bundled with the firmware. All we need to do is use a firmware downloading tool like SamFirm or Frija and download the firmware for our phone (SM-G9860, TGY). Unpacking the firmware, we will get 5 tar files. We can find the hypervisor binary in the BL archive. It's called hyp.mbn.lz4. 

Lucky for us, this binary is a ELF, so we know its load address as well as its entrypoint - Unlike Exynos >:(

Note you can find a relatively up-to-date ghidra project `here <https://github.com/mineo333/Qualcomm-HypX/tree/main/ghidra_public>`_.

There are a few key values we can glean from main. Firstly, the value of ``vbar_el2``. The value of ``vbar_el2`` is ``0x80093000``. This will be extremely useful when reversing the SMC call handler. 

From here, there are 2 functions worth inspecting: ``hypx_register`` and ``sync_from_lower_el_aarch64``. 


uh_call Handler
-----------------

The entrypoint for the uh_call handler (Among other things) is ``sync_from_lower_el_aarch64``. This is simply ``vbar_el2 + 0x400``. According to the * `esr_el2 documentation page <https://developer.arm.com/documentation/ddi0601/2022-03/AArch64-Registers/ESR-EL2--Exception-Syndrome-Register--EL2->`_, the EC value for an SMC is ``0b010111`` or ``0x17``.

This function is massive. However, the function care about is the call to ``dispatch_uh_call_or_tz_call``. This actually makes the call to the various HypX apps. Within ``dispatch_uh_call_or_tz_call``, we make a call to the function pointer of ``uh_call_handler``. However, what is the value of this function pointer? To get the answer to that we need to understand HypX is registered.


HypX Register
----------------

Registering HypX is a big part of how SMCs are handled. HypX registration happens in a function called ``hypx_register``. Within ``hypx_register``, the main registration occurs in a function pointer called ``hypx_setup_function``. The way this function pointer is retrieved is via the property subsystem. However, in short, this pointer points to ``0x80000000``. The function at this address (Also called ``hypx_setup_function``) sets the function pointer to ``uh_call_handler`` as well as a few other variables.









