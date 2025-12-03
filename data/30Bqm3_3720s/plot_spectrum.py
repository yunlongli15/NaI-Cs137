import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# 读取数据（跳过注释行）
df = pd.read_csv('nai_simulation_nt_GammaSpectrum.csv', comment='#', header=None, names=['EnergyDeposit', 'EventID', 'X', 'Y', 'Z'])

# 提取能量沉积数据并转换为keV
energy_mev = df['EnergyDeposit']
energy_kev = energy_mev * 1000  # MeV转换为keV

# 统计大于630 keV的计数
threshold = 630  # keV
counts_above_630 = np.sum(energy_kev > threshold)

# 计算总计数
total_counts = len(energy_kev)

# 计算比例
percentage_above_630 = (counts_above_630 / total_counts) * 100

print("=" * 60)
print("能量大于630 keV的统计结果")
print("=" * 60)
print(f"总事件数: {total_counts}")
print(f"能量 > 630 keV 的事件数: {counts_above_630}")
print(f"占比: {percentage_above_630:.2f}%")
print(f"平均能量: {energy_kev.mean():.2f} keV")

# 绘制能谱图（原始代码）
plt.figure(figsize=(12, 7))
plt.hist(energy_kev, bins=300, range=(0, 700), alpha=0.7, color='blue', edgecolor='black', log=True)
plt.xlabel('Energy Deposit (keV)', fontsize=12)
plt.ylabel('Counts (log scale)', fontsize=12)
plt.title('Gamma Spectrum (keV, log scale)', fontsize=14)
plt.grid(True, linestyle='--', alpha=0.7)

# 添加630 keV标记线
plt.axvline(x=630, color='red', linestyle='--', linewidth=2, alpha=0.8)
plt.text(630, plt.ylim()[1]*0.9, '630 keV', rotation=90, va='top', ha='right', fontsize=10, fontweight='bold', color='red')

# 添加统计信息文本框
stats_text = f"Total: {total_counts}\n>630 keV: {counts_above_630}\n({percentage_above_630:.1f}%)"
plt.text(0.75, 0.95, stats_text, transform=plt.gca().transAxes,
         verticalalignment='top', fontsize=11,
         bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))

plt.tight_layout()
plt.show()

# 打印一些统计信息（原始代码）
print(f"\n能量范围: {energy_kev.min():.2f} - {energy_kev.max():.2f} keV")
print(f"平均能量: {energy_kev.mean():.2f} keV")

