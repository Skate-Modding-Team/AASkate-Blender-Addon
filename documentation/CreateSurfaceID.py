import tkinter as tk
from tkinter import ttk, messagebox

# --- ENUM DEFINITIONS ---

AudioSurfaceType = [
    "Undefined", "Asphalt_Smooth", "Asphalt_Rough", "Concrete_Polished", "Concrete_Rough",
    "Concrete_Aggregate", "Wood_Ramp", "Plywood", "Dirt", "Metal", "Grass",
    "Metal_Solid_Round_1", "Metal_Solid_Round_1_Up", "Metal_Solid_Round_2", "Metal_Solid_Square_1",
    "Metal_Solid_Square_2", "Metal_Hollow_Round_1", "Metal_Hollow_Round_1_Dead", "Metal_Hollow_Round_1_Dn",
    "Metal_Hollow_Round_2", "Metal_Hollow_Round_2_Dead", "Metal_Hollow_Round_2_Dn",
    "Metal_Hollow_Round_3", "Metal_Hollow_Round_4", "Metal_Hollow_Square_1", "Metal_Hollow_Square_2",
    "Metal_Hollow_Square_3", "Metal_Hollow_Square_3_Dead", "Metal_Hollow_Square_4",
    "Metal_Hollow_1", "Metal_Hollow_2", "Metal_Sheet", "Metal_Complex_1", "Metal_Complex_2",
    "Metal_Complex_3", "Metal_Complex_4", "Metal_Complex_5", "Metal_Complex_6", "Metal_Complex_7",
    "Metal_Complex_8", "Metal_Complex_Debris", "Wood_1", "Wood_1_Up", "Wood_2", "Wood_3",
    "Wood_3_Up", "Wood_4", "Plastic_1", "Plastic_2", "Plastic_3", "Plastic_4", "Glass_Thick_Large",
    "Glass_Thin_Small", "Concrete_Curb", "Concrete_Bench", "Leaves", "Bush", "Pottery", "Paper",
    "Cardboard", "Garbage_Bag", "Garbage_Spill", "Bottle", "Tile_Ceramic", "Marble_or_Slate",
    "Brick_Smooth", "Brick_Coarse", "Manhole_Metal", "Metal_Grate_Sewer", "Metal_Grate_Planter",
    "DeepSnow", "PackedSnow", "Ice", "Antennas", "Chandelier", "Plexiglass_Small",
    "Plexiglass_Large", "Potted_Plant", "Crumpled_Paper", "Cloth", "Pop_Can", "Paper_Cup",
    "Wire_Cable", "VolleyBall", "OilDrum", "DMORail", "Fruit", "Plastic_Bottle", "Drum_Pylon",
    "Metal_Rail_4", "Wood_5", "Metal_Ramp", "Complex_Plastic_1", "Max_Mappable_Surface", "Board",
    "Truck", "Head", "Torso", "Leg", "Arm", "Foot", "HOMHead", "HOMTorso", "HOMLeg", "HOMArm",
    "HOMFoot", "Skin", "Denim", "Cotton", "BoneCrack", "BoneSnap", "FaceHit", "BoardTumble",
    "Metal_Hollow_Square_3_Dead_Rattle", "Metal_Hollow_1_Rattle", "Metal_Hollow_Square_3_Rattle",
    "Metal_Hollow_2_Rattle", "DMORail_Rattle", "Antennas_Rattle", "Metal_Complex_5_Rattle",
    "Metal_Sheet_Rattle", "Metal_Complex_1_Rattle", "Wood_1_Rattle", "Wood_2_Rattle",
    "Wood_3_Rattle", "Wood_4_Rattle", "Plastic_1_Rattle", "Plastic_2_Rattle", "Plastic_3_Rattle",
    "Plastic_4_Rattle", "Cardboard_Rattle", "Pottery_Rattle", "Pottery_2_Rattle"
]

PhysicsSurfaceType = [
    "Undefined", "Smooth", "Rough", "Slow", "Slippery", "VerySlow",
    "Unrideable", "DoNotAlign", "Stair"
]

SurfacePatternType = [
    "None", "SpiderCrack", "Square2x2", "Square4x4", "Square8x8", "Square12x12",
    "Square24x24", "IrregularSmall", "IrregularMedium", "IrregularLarge", "Slats",
    "Sidewalk", "BrickTileRandomSize", "MiniTile", "Special1", "Special2"
]

# --- CONSTANTS ---
AUDIO_BITS = 7
PHYSICS_BITS = 5
PATTERN_BITS = 5

PHYSICS_SHIFT = AUDIO_BITS              # 7
PATTERN_SHIFT = AUDIO_BITS + PHYSICS_BITS  # 12

# --- CALCULATE FUNCTION ---
def calculate_surface_id():
    try:
        audio_index = AudioSurfaceType.index(audio_combo.get())
        physics_index = PhysicsSurfaceType.index(physics_combo.get())
        pattern_index = SurfacePatternType.index(pattern_combo.get())
    except ValueError:
        messagebox.showerror("Error", "Invalid selection in one of the dropdowns.")
        return

    # Compose bitfield
    surface_id = (pattern_index << PATTERN_SHIFT) | (physics_index << PHYSICS_SHIFT) | audio_index
    surface_id &= 0xFFFFFFFF  # ensure uint32

    result_var.set(f"{surface_id} (0x{surface_id:08X})")

# --- DECODE FUNCTION ---
def decode_surface_id():
    value = decode_entry.get().strip()
    if not value:
        messagebox.showwarning("No Input", "Please enter a decimal Surface ID value.")
        return

    try:
        surface_id = int(value)
    except ValueError:
        messagebox.showerror("Error", "Please enter a valid integer value.")
        return

    # Extract fields
    audio_index   = (surface_id >> 0) & ((1 << AUDIO_BITS) - 1)
    physics_index = (surface_id >> PHYSICS_SHIFT) & ((1 << PHYSICS_BITS) - 1)
    pattern_index = (surface_id >> PATTERN_SHIFT) & ((1 << PATTERN_BITS) - 1)

    # Update combobox selections safely
    if audio_index < len(AudioSurfaceType):
        audio_combo.set(AudioSurfaceType[audio_index])
    else:
        audio_combo.set("Undefined")

    if physics_index < len(PhysicsSurfaceType):
        physics_combo.set(PhysicsSurfaceType[physics_index])
    else:
        physics_combo.set("Undefined")

    if pattern_index < len(SurfacePatternType):
        pattern_combo.set(SurfacePatternType[pattern_index])
    else:
        pattern_combo.set("None")

    # Also show the formatted value
    result_var.set(f"{surface_id} (0x{surface_id:08X})")

# --- COPY FUNCTION ---
def copy_to_clipboard():
    value = result_var.get()
    if value:
        root.clipboard_clear()
        root.clipboard_append(value)
        messagebox.showinfo("Copied", f"Copied to clipboard:\n{value}")
    else:
        messagebox.showwarning("No Value", "Please generate a Surface ID first.")

# --- UI SETUP ---
root = tk.Tk()
root.title("Surface ID Generator / Decoder")
root.geometry("720x480")

frame = ttk.Frame(root, padding=20)
frame.pack(fill="both", expand=True)

ttk.Label(frame, text="Audio Surface Type:").grid(row=0, column=0, sticky="w", pady=5)
audio_combo = ttk.Combobox(frame, values=AudioSurfaceType, width=40)
audio_combo.set(AudioSurfaceType[0])
audio_combo.grid(row=0, column=1, pady=5)

ttk.Label(frame, text="Physics Surface Type:").grid(row=1, column=0, sticky="w", pady=5)
physics_combo = ttk.Combobox(frame, values=PhysicsSurfaceType, width=40)
physics_combo.set(PhysicsSurfaceType[0])
physics_combo.grid(row=1, column=1, pady=5)

ttk.Label(frame, text="Surface Pattern Type:").grid(row=2, column=0, sticky="w", pady=5)
pattern_combo = ttk.Combobox(frame, values=SurfacePatternType, width=40)
pattern_combo.set(SurfacePatternType[0])
pattern_combo.grid(row=2, column=1, pady=5)

# Buttons
calc_btn = ttk.Button(frame, text="Generate Surface ID", command=calculate_surface_id)
calc_btn.grid(row=3, column=0, columnspan=2, pady=15)

# Result row
result_var = tk.StringVar()
ttk.Label(frame, text="Surface ID:").grid(row=4, column=0, sticky="w")
result_entry = ttk.Entry(frame, textvariable=result_var, font=("Consolas", 14), width=30, state="readonly")
result_entry.grid(row=4, column=1, sticky="w")
copy_btn = ttk.Button(frame, text="Copy", command=copy_to_clipboard)
copy_btn.grid(row=4, column=2, padx=10)

# --- Decoder Section ---
ttk.Separator(frame, orient="horizontal").grid(row=5, column=0, columnspan=3, sticky="ew", pady=15)
ttk.Label(frame, text="Decode from Decimal Value:").grid(row=6, column=0, sticky="w", pady=5)
decode_entry = ttk.Entry(frame, width=30)
decode_entry.grid(row=6, column=1, sticky="w", pady=5)
decode_btn = ttk.Button(frame, text="Decode", command=decode_surface_id)
decode_btn.grid(row=6, column=2, padx=10)

root.mainloop()
