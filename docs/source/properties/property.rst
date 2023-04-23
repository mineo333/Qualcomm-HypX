The Property Subsystem
=======================

The property subsystem or DAL (Data Access Layer) is a key subsystem within the Qualcomm Hypervisor as it contains key addresses for structures and functions such as the uh_call entrypoint. Without a robust understanding of the property subsystem, its impossible to go anywhere important.

Functionally, the property subsystem is a tree.

Property Subsystem Inner Workings
--------------------------------------

Before we attempt to actually get any properties, we need to call ``get_prop_devcfg`` or ``get_prop_hyp``. These setup the ``property_desc`` for property retrieval.

So, the question is, why are there two functions? The reason for this is that there are two places where properties can be located: in the hypervisor binary itself or in the external devcfg binary. If the property is in devcfg, we use ``get_prop_devcfg``. If the property is in the hypervisor, we use ``get_prop_hyp``. Regardless, these functions both do very similar things. 

The first thing they do is get the ``pDal``. This structure can be thought as the root of the tree. This structure also contains a few elements. The first element is what I call the ``prop_base``. This is where the property search starts. The second known element is the ``PropBase`` array address. The third element is the ``PropBase`` array size. The struct definition can be found below:

.. code-block:: C
    :caption: pDal

    struct pDAL {
        long * prop_base;
        long unknown;
        int PropBase_arr_size;
        undefined field3_0x14;
        undefined field4_0x15;
        undefined field5_0x16;
        undefined field6_0x17;
        struct PropBase * PropBase_arr;
    };


``PropBase`` is critical for the ``get_prop_*`` functions. Below is the struct definition:

.. code-block:: C
    :caption: PropBase

    struct PropBase {
        char * name;
        int id;
        int pDalPropsDir/pbDevicePropspos;
        undefined field3_0x10;
        undefined field4_0x11;
        undefined field5_0x12;
        undefined field6_0x13;
        undefined field7_0x14;
        undefined field8_0x15;
        undefined field9_0x16;
        undefined field10_0x17;
        undefined field11_0x18;
        undefined field12_0x19;
        undefined field13_0x1a;
        undefined field14_0x1b;
        undefined field15_0x1c;
        undefined field16_0x1d;
        undefined field17_0x1e;
        undefined field18_0x1f;
        undefined field19_0x20;
        undefined field20_0x21;
        undefined field21_0x22;
        undefined field22_0x23;
        undefined field23_0x24;
        undefined field24_0x25;
        undefined field25_0x26;
        undefined field26_0x27;
    };

This structure contains the information for each property category. The ``PropBase`` structure can be thought of as tree nodes at a depth of 1. Each property is under a ``PropBase`` and each ``PropBase`` is identified by a name. It is the purpose of ``get_prop_*`` to find the ``PropBase`` given a name. The way it does this is by calculating a checksum of the name argument. This checksum is then compared with the ID of each ``PropBase`` in the ``PropBase`` array. When the id is the same, the correct ``PropBase`` is found. At this point, the ``property_desc`` can be filled. Below is the struct definition for ``property_desc``:

.. code-block:: C
    :caption: property_desc

    struct property_desc {
        struct pDAL * DALPROP_PropsInfo;
        long pDalPropsDir/pbDevicePropspos;
    };

In the above structure, ``DALPROP_PropsInfo`` is the ``pDAL`` we found earlier while ``pDalPropsDir/pbDevicePropspos`` is the ``pDalPropsDir/pbDevicePropspos`` from the matching ``PropBase``. It is this structure that will allow us to find a property under a particular ``PropBase``. 

This is where ``find_property`` comes in. The inner workings of ``find_property`` are fairly opaque, but the idea is that we iterate through the properties under ``PropBase`` until we find the right property. The base address for the search if ``DALPROP_PropsInfo -> prop_base + pDalPropsDir/pbDevicePropspos``.