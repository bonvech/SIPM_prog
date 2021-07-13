//==========================================================
int SIPM_device::InitMinusPower(int RDAC,int comm,int data)
{
//return	WR_IIC(0x2E,0x7, 2,3, 0x021C2E);
    return	WR_IIC(RDAC,comm, data,3, 0x021C2E);
}

//==========================================================
int SIPM_device::SetMinusPower(int RDAC,int comm,int data)
{
//	WR_IIC(0x2E,0x7, 2,3, 0x021C2E);
//	WR_IIC(0x2E, 0x1, 0x10F,3,0x0F052E);
    WR_IIC(RDAC, comm, data,3,0x0F052E);
    return 1;
}

//==========================================================
int SIPM_device::InitPlusPower(int RDAC,int comm,int data)
{
//return	WR_IIC(0x2E,0x7, 2,3, 0x021C2E);
    return	WR_IIC(RDAC,comm, data,4, 0x0);
}

//==========================================================
int SIPM_device::SetPlusPower(int RDAC,int comm,int det, int data)
{
//	WR_IIC(0x2E,0x7, 2,3, 0x021C2E);
//	WR_IIC(0x2E, 0x1, 0x10F,3,0x0F052E);
    WR_IIC(RDAC, comm+det, data,4,0x0);
    return 1;
}
//==========================================================
