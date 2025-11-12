# runs after firmware upload; writes AssetPool.bin to the 'assetpool' partition
Import("env")
import os, subprocess

project_dir = env["PROJECT_DIR"]
assetpool_bin = os.path.join(project_dir, "AssetPool.bin")

def write_assetpool(source, target, env_):
    if not os.path.exists(assetpool_bin):
        print("warning: AssetPool.bin not found; skipping assetpool write")
        print("hint: build it once in the upstream repo: cmake .. && make -j; then copy build/desktop/AssetPool.bin here")
        return
    # pick an upload port (user can also set upload_port in platformio.ini)
    port = env_.GetProjectOption("upload_port") or (env_.GetSerialPorts() or [None])[0]
    if not port:
        print("warning: no serial port detected; cannot write assetpool")
        return
    cmd = [
        "parttool.py",
        "--port", port,
        "write_partition",
        "--partition-name=assetpool",
        "--input", assetpool_bin
    ]
    print("writing assetpool partition…")
    subprocess.check_call(cmd)

env.AddPostAction("upload", write_assetpool)
