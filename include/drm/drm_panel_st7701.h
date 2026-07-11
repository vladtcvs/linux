// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2019, Amarula Solutions.
 * Author: Jagan Teki <jagan@amarulasolutions.com>
 *         Vladislav Tsendrovskii <vtcendrovskii@gmail.com>
 */

#ifndef __DRM_PANEL_ST7701_H__
#define __DRM_PANEL_ST7701_H__

#include <drm/drm_mipi_dbi.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <linux/regulator/consumer.h>

/* Command2 BKx selection command */
#define ST7701_CMD2BKX_SEL          0xFF
#define ST7701_CMD1             0
#define ST7701_CMD2             BIT(4)
#define ST7701_CMD2BK_MASK          GENMASK(3, 0)
    
/* Command2, BK0 commands */
#define ST7701_CMD2_BK0_PVGAMCTRL       0xB0 /* Positive Voltage Gamma Control */
#define ST7701_CMD2_BK0_NVGAMCTRL       0xB1 /* Negative Voltage Gamma Control */
#define ST7701_CMD2_BK0_LNESET          0xC0 /* Display Line setting */
#define ST7701_CMD2_BK0_PORCTRL         0xC1 /* Porch control */
#define ST7701_CMD2_BK0_INVSEL          0xC2 /* Inversion selection, Frame Rate Control */
#define ST7701_CMD2_BK0_RGBCTRL         0xC3 /* RGB control */
#define ST7701_CMD2_BK0_SDIR            0xC7 /* X-direction Control */


/* Command2, BK1 commands */
#define ST7701_CMD2_BK1_VRHS            0xB0 /* Vop amplitude setting */
#define ST7701_CMD2_BK1_VCOM            0xB1 /* VCOM amplitude setting */
#define ST7701_CMD2_BK1_VGHSS           0xB2 /* VGH Voltage setting */
#define ST7701_CMD2_BK1_TESTCMD         0xB3 /* TEST Command Setting */
#define ST7701_CMD2_BK1_VGLS            0xB5 /* VGL Voltage setting */
#define ST7701_CMD2_BK1_PWCTLR1         0xB7 /* Power Control 1 */
#define ST7701_CMD2_BK1_PWCTLR2         0xB8 /* Power Control 2 */
#define ST7701_CMD2_BK1_SPD1            0xC1 /* Source pre_drive timing set1 */
#define ST7701_CMD2_BK1_SPD2            0xC2 /* Source EQ2 Setting */
#define ST7701_CMD2_BK1_MIPISET1        0xD0 /* MIPI Setting 1 */
    
/* Command2, BK0 bytes */   
#define ST7701_CMD2_BK0_GAMCTRL_AJ_MASK     GENMASK(7, 6)
#define ST7701_CMD2_BK0_GAMCTRL_VC0_MASK    GENMASK(3, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC4_MASK    GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC8_MASK    GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC16_MASK   GENMASK(4, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC24_MASK   GENMASK(4, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC52_MASK   GENMASK(3, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC80_MASK   GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC108_MASK  GENMASK(3, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC147_MASK  GENMASK(3, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC175_MASK  GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC203_MASK  GENMASK(3, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC231_MASK  GENMASK(4, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC239_MASK  GENMASK(4, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC247_MASK  GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC251_MASK  GENMASK(5, 0)
#define ST7701_CMD2_BK0_GAMCTRL_VC255_MASK  GENMASK(4, 0)
#define ST7701_CMD2_BK0_LNESET_LINE_MASK    GENMASK(6, 0)
#define ST7701_CMD2_BK0_LNESET_LDE_EN       BIT(7)
#define ST7701_CMD2_BK0_LNESET_LINEDELTA    GENMASK(1, 0)
#define ST7701_CMD2_BK0_PORCTRL_VBP_MASK    GENMASK(7, 0)
#define ST7701_CMD2_BK0_PORCTRL_VFP_MASK    GENMASK(7, 0)
#define ST7701_CMD2_BK0_INVSEL_ONES_MASK    GENMASK(5, 4)
#define ST7701_CMD2_BK0_INVSEL_NLINV_MASK   GENMASK(2, 0)
#define ST7701_CMD2_BK0_INVSEL_RTNI_MASK    GENMASK(4, 0)

/* Command2, BK1 bytes */
#define ST7701_CMD2_BK1_VRHA_MASK       GENMASK(7, 0)
#define ST7701_CMD2_BK1_VCOM_MASK       GENMASK(7, 0)
#define ST7701_CMD2_BK1_VGHSS_MASK      GENMASK(3, 0)
#define ST7701_CMD2_BK1_TESTCMD_VAL     BIT(7)
#define ST7701_CMD2_BK1_VGLS_ONES       BIT(6)
#define ST7701_CMD2_BK1_VGLS_MASK       GENMASK(3, 0)
#define ST7701_CMD2_BK1_PWRCTRL1_AP_MASK    GENMASK(7, 6)
#define ST7701_CMD2_BK1_PWRCTRL1_APIS_MASK  GENMASK(3, 2)
#define ST7701_CMD2_BK1_PWRCTRL1_APOS_MASK  GENMASK(1, 0)
#define ST7701_CMD2_BK1_PWRCTRL2_AVDD_MASK  GENMASK(5, 4)
#define ST7701_CMD2_BK1_PWRCTRL2_AVCL_MASK  GENMASK(1, 0)
#define ST7701_CMD2_BK1_SPD1_ONES_MASK      GENMASK(6, 4)
#define ST7701_CMD2_BK1_SPD1_T2D_MASK       GENMASK(3, 0)
#define ST7701_CMD2_BK1_SPD2_ONES_MASK      GENMASK(6, 4)
#define ST7701_CMD2_BK1_SPD2_T3D_MASK       GENMASK(3, 0)
#define ST7701_CMD2_BK1_MIPISET1_ONES       BIT(7)
#define ST7701_CMD2_BK1_MIPISET1_EOT_EN     BIT(3)

enum op_bias {
    OP_BIAS_OFF = 0,
    OP_BIAS_MIN,
    OP_BIAS_MIDDLE,
    OP_BIAS_MAX
};

struct st7701;

struct st7701_panel_desc {
    const struct drm_display_mode *mode;
    unsigned int lanes;
    enum mipi_dsi_pixel_format format;
    unsigned int panel_sleep_delay;

    /* TFT matrix driver configuration, panel specific. */
    const u8    pv_gamma[16];   /* Positive voltage gamma control */
    const u8    nv_gamma[16];   /* Negative voltage gamma control */
    const u8    nlinv;      /* Inversion selection */
    const u32   vop_uv;     /* Vop in uV */
    const u32   vcom_uv;    /* Vcom in uV */
    const u16   vgh_mv;     /* Vgh in mV */
    const s16   vgl_mv;     /* Vgl in mV */
    const u16   avdd_mv;    /* Avdd in mV */
    const s16   avcl_mv;    /* Avcl in mV */
    const enum op_bias  gamma_op_bias;
    const enum op_bias  input_op_bias;
    const enum op_bias  output_op_bias;
    const u16   t2d_ns;     /* T2D in ns */
    const u16   t3d_ns;     /* T3D in ns */
    const bool  eot_en;

    /* GIP sequence, fully custom and undocumented. */
    void        (*gip_sequence)(struct st7701 *st7701);
};

struct st7701 {
    struct drm_panel panel;
    struct mipi_dsi_device *dsi;
    struct mipi_dbi dbi;
    const struct st7701_panel_desc *desc;

    struct regulator_bulk_data supplies[2];
    struct gpio_desc *reset;
    unsigned int sleep_delay;
    enum drm_panel_orientation orientation;

    int (*write_command)(struct st7701 *st7701, u8 cmd, const u8 *seq,
                 size_t len);
};

#define CFIELD_PREP(_mask, _val)					\
	(((typeof(_mask))(_val) << (__builtin_ffsll(_mask) - 1)) & (_mask))

#define ST7701_WRITE(st7701, cmd, seq...)				\
	{								\
		const u8 d[] = { seq };					\
		st7701->write_command(st7701, cmd, d, ARRAY_SIZE(d));	\
	}

void st7701_switch_cmd_bkx(struct st7701 *st7701, bool cmd2, u8 bkx);

#endif // __DRM_PANEL_ST7701_H__
