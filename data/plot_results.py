import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 根据Excel表格数据手动输入（直接读取表格内容）
data = {
    'Activity_Concentration_Bq_m3': [10, 20, 30, 40, 50, 100],
    'Peak_Counts': [386, 762, 1017, 1551, 1739, 3572],  # I列
    'Total_Events': [9742134, 19473628, 29217582, 38955053, 48683810, 97390244],  # J列
    # 计算探测效率 K列 = I/J
    'Detection_Efficiency': [386/9742134, 762/19473628, 1017/29217582, 
                            1551/38955053, 1739/48683810, 3572/97390244],
    # 计算探测限 L列 = 4.66*1*SQRT($D$2)/($H$2*$F$2*K2*$C$2)
    # 其中：D2=4979（本底计数）, H2=3200（模拟时长）, F2=体积, C2=0.8998（发射几率）
}

# 计算体积（半球体积: 2/3 * π * r³, r=5m）
volume = (2/3) * np.pi * (5**3)
background_counts = 4979
simulation_time = 3200
emission_probability = 0.8998

# 计算探测限
detection_limits = []
for eff in data['Detection_Efficiency']:
    dl = (4.66 * 1 * np.sqrt(background_counts)) / (simulation_time * volume * eff * emission_probability)
    detection_limits.append(dl)

data['Detection_Limit_Bq_m3'] = detection_limits

# 创建DataFrame
df = pd.DataFrame(data)

print("=" * 80)
print("Cs-137 探测效率和探测限数据表")
print("=" * 80)
print(f"{'活度浓度(Bq/m³)':<15} {'探测效率':<20} {'探测限(Bq/m³)':<15}")
print("-" * 80)
for i in range(len(df)):
    print(f"{df['Activity_Concentration_Bq_m3'][i]:<15.0f} "
          f"{df['Detection_Efficiency'][i]:<20.2e} "
          f"{df['Detection_Limit_Bq_m3'][i]:<15.3f}")

# 计算统计量
detection_efficiency_mean = df['Detection_Efficiency'].mean()
detection_efficiency_std = df['Detection_Efficiency'].std()
detection_efficiency_relative_error = (detection_efficiency_std / detection_efficiency_mean) * 100

detection_limit_mean = df['Detection_Limit_Bq_m3'].mean()
detection_limit_std = df['Detection_Limit_Bq_m3'].std()
detection_limit_relative_error = (detection_limit_std / detection_limit_mean) * 100

print("\n" + "=" * 80)
print("统计结果")
print("=" * 80)
print(f"探测效率:")
print(f"  平均值: {detection_efficiency_mean:.3e}")
print(f"  标准差: {detection_efficiency_std:.3e}")
print(f"  相对误差: {detection_efficiency_relative_error:.2f}%")
print(f"  范围: {df['Detection_Efficiency'].min():.2e} - {df['Detection_Efficiency'].max():.2e}")

print(f"\n探测限:")
print(f"  平均值: {detection_limit_mean:.3f} Bq/m³")
print(f"  标准差: {detection_limit_std:.3f} Bq/m³")
print(f"  相对误差: {detection_limit_relative_error:.2f}%")
print(f"  范围: {df['Detection_Limit_Bq_m3'].min():.3f} - {df['Detection_Limit_Bq_m3'].max():.3f} Bq/m³")

# 创建双纵轴图
fig, ax1 = plt.figure(figsize=(14, 8)), plt.gca()

# 设置颜色
color1 = 'tab:blue'
color2 = 'tab:red'

# 第一个y轴：探测效率（左轴）
ax1.set_xlabel('Activity Concentration (Bq/m³)', fontsize=14, fontweight='bold')
ax1.set_ylabel('Detection Efficiency', fontsize=14, fontweight='bold', color=color1)

# 绘制探测效率曲线（左轴）
line1 = ax1.plot(df['Activity_Concentration_Bq_m3'], df['Detection_Efficiency'], 
                 'o-', color=color1, linewidth=3, markersize=10,
                 markerfacecolor=color1, markeredgecolor='black', markeredgewidth=1.5,
                 label='Detection Efficiency')

ax1.tick_params(axis='y', labelcolor=color1)
ax1.set_yscale('log')

# 扩大探测效率纵轴范围
efficiency_min = df['Detection_Efficiency'].min()
efficiency_max = df['Detection_Efficiency'].max()
ax1.set_ylim([efficiency_min * 0.5, efficiency_max * 2.0])

# 设置x轴范围
plt.xlim([5, 105])

# 添加网格
ax1.grid(True, alpha=0.3, linestyle='--', which='both')

# 在探测效率数据点上添加数值标签
for i, (x, y) in enumerate(zip(df['Activity_Concentration_Bq_m3'], df['Detection_Efficiency'])):
    ax1.annotate(f'{y:.2e}', xy=(x, y), xytext=(0, 10), 
                 textcoords='offset points', ha='center', va='bottom',
                 fontsize=10, fontweight='bold', color=color1,
                 bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.8))

# 添加探测效率平均值线
ax1.axhline(y=detection_efficiency_mean, color=color1, linestyle=':', linewidth=2, alpha=0.7)

# 第二个y轴：探测限（右轴）
ax2 = ax1.twinx()
ax2.set_ylabel('Detection Limit (Bq/m³)', fontsize=14, fontweight='bold', color=color2)

# 绘制探测限曲线（右轴）
line2 = ax2.plot(df['Activity_Concentration_Bq_m3'], df['Detection_Limit_Bq_m3'], 
                 's--', color=color2, linewidth=2.5, markersize=10,
                 markerfacecolor=color2, markeredgecolor='black', markeredgewidth=1.5,
                 label='Detection Limit')

ax2.tick_params(axis='y', labelcolor=color2)

# 扩大探测限纵轴范围
limit_min = df['Detection_Limit_Bq_m3'].min()
limit_max = df['Detection_Limit_Bq_m3'].max()
ax2.set_ylim([limit_min * 0.8, limit_max * 1.2])

# 在探测限数据点上添加数值标签
for i, (x, y) in enumerate(zip(df['Activity_Concentration_Bq_m3'], df['Detection_Limit_Bq_m3'])):
    ax2.annotate(f'{y:.3f}', xy=(x, y), xytext=(0, -15), 
                 textcoords='offset points', ha='center', va='top',
                 fontsize=10, fontweight='bold', color=color2,
                 bbox=dict(boxstyle="round,pad=0.2", facecolor="white", alpha=0.8))

# 添加探测限平均值线
ax2.axhline(y=detection_limit_mean, color=color2, linestyle=':', linewidth=2, alpha=0.7)

# 添加图例
lines = line1 + line2
labels = [l.get_label() for l in lines]
ax1.legend(lines, labels, loc='upper left', fontsize=12, framealpha=0.9)

# 添加统计信息标注
efficiency_stats = f"Detection Efficiency:\n"
efficiency_stats += f"Mean: {detection_efficiency_mean:.2e}\n"
efficiency_stats += f"Std: {detection_efficiency_std:.2e}"

limit_stats = f"Detection Limit:\n"
limit_stats += f"Mean: {detection_limit_mean:.3f} Bq/m³\n"
limit_stats += f"Std: {detection_limit_std:.3f} Bq/m³"

# 在图上添加统计信息
ax1.text(0.02, 0.98, efficiency_stats, fontsize=11, verticalalignment='top',
         transform=ax1.transAxes, color=color1,
         bbox=dict(boxstyle="round,pad=0.3", facecolor="lightblue", alpha=0.7))

ax1.text(0.02, 0.82, limit_stats, fontsize=11, verticalalignment='top',
         transform=ax1.transAxes, color=color2,
         bbox=dict(boxstyle="round,pad=0.3", facecolor="mistyrose", alpha=0.7))

# 调整布局
plt.tight_layout()

# 保存图像
plt.savefig('detection_efficiency_and_limit.png', dpi=300, bbox_inches='tight')

# 显示图像
plt.show()
