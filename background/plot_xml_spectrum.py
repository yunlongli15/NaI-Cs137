import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt
import numpy as np
import re

def parse_n42_spectrum(file_path):
    """
    解析N42格式的核能谱XML文件
    """
    try:
        # 读取并解析XML文件
        tree = ET.parse(file_path)
        root = tree.getroot()
    except ET.ParseError as e:
        print(f"XML解析错误: {e}")
        return None, None, None
    except FileNotFoundError:
        print(f"文件未找到: {file_path}")
        return None, None, None
    
    # 注册命名空间
    namespaces = {
        'n42': 'http://physics.nist.gov/Divisions/Div846/Gp4/ANSIN4242/2005/ANSIN4242'
    }
    
    # 查找Spectrum元素
    spectrum = root.find('.//n42:Spectrum', namespaces)
    if spectrum is None:
        print("未找到Spectrum数据")
        return None, None, None
    
    # 获取通道数据
    channel_data_elem = spectrum.find('n42:ChannelData', namespaces)
    if channel_data_elem is None:
        print("未找到ChannelData")
        return None, None, None
    
    # 解析通道数据（去除换行和多余空格，分割成列表）
    channel_text = channel_data_elem.text
    channel_text = re.sub(r'\s+', ' ', channel_text.strip())  # 标准化空格
    channel_counts = [int(x) for x in channel_text.split()]
    
    # 获取能量校准信息
    calibration = spectrum.find('n42:Calibration', namespaces)
    energy_calibration = None
    
    if calibration is not None and calibration.get('Type') == 'Energy':
        equation = calibration.find('n42:Equation', namespaces)
        if equation is not None:
            coefficients_elem = equation.find('n42:Coefficients', namespaces)
            if coefficients_elem is not None:
                coeffs = [float(x) for x in coefficients_elem.text.split()]
                if len(coeffs) >= 2:
                    # E = Term0 + Term1*Ch
                    energy_calibration = lambda ch: coeffs[0] + coeffs[1] * ch
    
    # 获取测量信息
    real_time = spectrum.find('n42:RealTime', namespaces)
    live_time = spectrum.find('n42:LiveTime', namespaces)
    start_time = spectrum.find('n42:StartTime', namespaces)
    
    info = {
        'real_time': real_time.text if real_time is not None else 'Unknown',
        'live_time': live_time.text if live_time is not None else 'Unknown',
        'start_time': start_time.text if start_time is not None else 'Unknown',
        'detector': spectrum.get('Detector', 'Unknown'),
        'channel_count': len(channel_counts)
    }
    
    return channel_counts, energy_calibration, info

def plot_spectrum(channel_counts, energy_calibration, info):
    """
    绘制能谱图
    """
    # 创建通道号数组
    channels = np.arange(len(channel_counts))
    
    # 创建图形
    plt.figure(figsize=(12, 6))
    
    if energy_calibration:
        # 如果有能量校准，使用能量作为X轴
        energies = energy_calibration(channels)
        plt.semilogy(energies, channel_counts, linewidth=1, color='blue')
        plt.xlabel('Energy (keV)', fontsize=12)
        plt.title('Gamma Spectrum - Energy Scale', fontsize=14)
    else:
        # 如果没有能量校准，使用通道号作为X轴
        plt.semilogy(channels, channel_counts, linewidth=1, color='blue')
        plt.xlabel('Channel Number', fontsize=12)
        plt.title('Gamma Spectrum - Channel Scale', fontsize=14)
    
    plt.ylabel('Counts', fontsize=12)
    plt.grid(True, which="both", ls="--", alpha=0.3)
    
    # 添加测量信息文本框
    info_text = f"Detector: {info['detector']}\n"
    info_text += f"Start Time: {info['start_time']}\n"
    info_text += f"Live Time: {info['live_time']}\n"
    info_text += f"Channels: {info['channel_count']}"
    
    plt.annotate(info_text, xy=(0.02, 0.98), xycoords='axes fraction',
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray", alpha=0.8),
                verticalalignment='top', fontsize=10)
    
    plt.tight_layout()
    plt.show()

# 使用示例
if __name__ == "__main__":
    # 直接指定文件路径
    file_path = "RS250_19207_20170218_135001_BG.xml"  # 替换为您的实际文件路径
    
    # 解析能谱数据
    channel_counts, energy_calibration, info = parse_n42_spectrum(file_path)
    
    if channel_counts is not None:
        print(f"成功读取能谱数据，共 {len(channel_counts)} 个通道")
        print(f"测量时间: {info['live_time']}")
        print(f"探测器: {info['detector']}")
        
        # 绘制能谱图
        plot_spectrum(channel_counts, energy_calibration, info)
    else:
        print("读取能谱数据失败")