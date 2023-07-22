# big-dripper
Water curtain display controller and MCU code

# Organization
- `/big_dripper/` - All application code for linux rpi application lives  (probably python application)
- `requirements.txt` - All python requirements should be tracked here
- `/share/` - All other configurations files for linux live here (ex. init.d scripts /boot/config)
- `/dripperator/` - PlatformIO project for pump IO board control

## Setup For Python App Development
clone this git repository and then run (make sure you have pip and venv installed):

``` bash
python -m venv venv
source venv/bin/activate
pip install -r linux/python/requirements.txt
```

Then point your development enviroment to venv.

## How to talk to a driperator
First, configure the DIP switches of the dripperator boards so they are in ascending address order from left to right starting at zero.

### As a library
```py
from hardware.dripperator_driver import DripperatorDriver
dripperator = DripperatorDriver("/dev/serial0", 17, your_num_driperators_here)
dripperator.display_row(bytes.fromhex("00"))
```

### From interactive python on big dripper
```bash
cd ~/big_dripper
python -i hardware/dripperator_driver.py
>>> dripperator.dripperator.display_row(bytes.fromhex("00"))
```