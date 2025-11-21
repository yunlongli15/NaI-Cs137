import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取数据（跳过注释行）
df = pd.read_csv('build/nai_simulation_nt_GammaSpectrum.csv', comment='#', header=None, names=['EnergyDeposit', 'EventID', 'X', 'Y', 'Z'])

# 提取能量沉积数据并转换为keV
energy_mev = df['EnergyDeposit']
energy_kev = energy_mev * 1000  # MeV转换为keV

# 绘制能谱图
plt.figure(figsize=(12, 7))
plt.hist(energy_kev, bins=300, range=(0, 700), alpha=0.7, color='blue', edgecolor='black', log=True)
plt.xlabel('Energy Deposit (keV)', fontsize=12)
plt.ylabel('Counts (log scale)', fontsize=12)
plt.title('Gamma Spectrum (keV, log scale)', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)

# 添加一些关键能量标记（常见的伽马射线能量）
common_energies = [122, 511, 662, 1173, 1332]  # 常见伽马射线能量(keV)
for e in common_energies:
    plt.axvline(x=e, color='red', linestyle='--', alpha=0.5, linewidth=1)
    plt.text(e, plt.ylim()[1]*0.9, f'{e} keV', rotation=90, va='top', ha='right', fontsize=8)

plt.tight_layout()
plt.show()
#plt.savefig("gamma_spectrum.png", dpi=300)

# 打印一些统计信息
print(f"能量范围: {energy_kev.min():.2f} - {energy_kev.max():.2f} keV")
print(f"总事件数: {len(energy_kev)}")
print(f"平均能量: {energy_kev.mean():.2f} keV")
