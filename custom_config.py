Import("env")

board_config = env.BoardConfig()
board_config.update("build.usb_product", "Footpedals")
