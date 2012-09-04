WRITE_REG 0x00E3, 0x3008
WRITE_REG 0x00E7, 0x0012
WRITE_REG 0x00EF, 0x1231 // set the internal timing;    
WRITE_REG 0x0000, 0x0001 // start fosc
WRITE_REG 0x0001, 0x0100 // set SS bit
WRITE_REG 0x0002, 0x0700 // set 1 line inversion
WRITE_REG 0x0003, 0x1018 // gram direction

WRITE_REG 0x0004, 0x0000 // Resize register
WRITE_REG 0x0008, 0x0202 // set the back porch and front porch
WRITE_REG 0x0009, 0x0000 // set non-display area refresh cycle ISC[3:0]
WRITE_REG 0x000A, 0x0000 // FMARK function
WRITE_REG 0x000C, 0x0000 // RGB interface setting
WRITE_REG 0x000D, 0x0000 // Frame marker Position
WRITE_REG 0x000F, 0x0000 // RGB interface polarity

//Power On sequence //
WRITE_REG 0x0010, 0x0000 // SAP, BT[3:0], AP, DSTB, SLP, STB
WRITE_REG 0x0011, 0x0007 // DC1[2:0], DC0[2:0], VC[2:0]
WRITE_REG 0x0012, 0x0000 // VREG1OUT voltage
WRITE_REG 0x0013, 0x0000 // VDV[4:0] for VCOM amplitude
rcall	delay
rcall	delay

WRITE_REG 0x0010, 0x1490 // SAP, BT[3:0], AP, DSTB, SLP, STB
WRITE_REG 0x0011, 0x0227 // DC1[2:0], DC0[2:0], VC[2:0]
rcall	delay

WRITE_REG 0x0012, 0x001C // VREG1OUT voltage
rcall	delay

WRITE_REG 0x0013, 0x1A00 // VDV[4:0] for VCOM amplitude
WRITE_REG 0x0029, 0x0025 // Set VCM[5:0] for VCOMH
WRITE_REG 0x002B, 0x000C // Set Frame Rate
rcall	delay

WRITE_REG 0x0020, 0x0000 // GRAM horizontal
WRITE_REG 0x0021, 0x0000 // GRAM vertical

// Adjust the Gamma Curve
WRITE_REG 0x0030, 0x0107
WRITE_REG 0x0031, 0x0306
WRITE_REG 0x0032, 0x0207
WRITE_REG 0x0035, 0x0206
WRITE_REG 0x0036, 0x0408
WRITE_REG 0x0037, 0x0106
WRITE_REG 0x0038, 0x0102
WRITE_REG 0x0039, 0x0207
WRITE_REG 0x003C, 0x0504
WRITE_REG 0x003D, 0x0503

// Set GRAM area
WRITE_REG 0x0050, 0x0000 // Horizontal GRAM Start Address
WRITE_REG 0x0051, 0x00EF // Horizontal GRAM End Address
WRITE_REG 0x0052, 0x0000 // Vertical GRAM Start Address
WRITE_REG 0x0053, 0x013F // Vertical GRAM Start Address
WRITE_REG 0x0060, 0xA700 // Gate Scan Line 0xA700
WRITE_REG 0x0061, 0x0001 // NDL, VLE, REV
WRITE_REG 0x006A, 0x0000 // set scrolling line

// Partial Display Control
WRITE_REG 0x0080, 0x0000
WRITE_REG 0x0081, 0x0000
WRITE_REG 0x0082, 0x0000
WRITE_REG 0x0083, 0x0000
WRITE_REG 0x0084, 0x0000
WRITE_REG 0x0085, 0x0000

// Panel Control
WRITE_REG 0x0090, 0x0010
WRITE_REG 0x0092, 0x0600

// Turn Display ON 
WRITE_REG 0x0007, 0x0133 // 262K color and display ON
