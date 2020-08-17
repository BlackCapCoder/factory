- visual mode
- yank/paste/registers

- machines
- longer undergroundee variants- should be weaveable
- belt lanes
- graphics work


# Bugs

- Apparently SDL viewports doesn't support negative coordinates
- Belts are slightly too fast:
  A belt may receive a tick after and on the same frame that
  it has been given an item.
  It incorrectly assumes that it must have had the item since
  the last frame, moving it slightly.
