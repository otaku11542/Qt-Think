from fuzzywuzzy import process
import difflib
from itertools import permutations
import time


# 如果【】中无信息，删除【】
def remove_empty_brackets(s):
    result = []
    i = 0
    while i < len(s):
        if s[i] == '【':
            # 找到对应的】
            j = s.find('】', i)
            if j != -1:
                # 检查【】对中是否存在字母或数字
                if not any(c.isalnum() for c in s[i + 1:j]):
                    # 如果不存在，则跳过这个【】对
                    i = j + 1
                    continue
                else:
                    # 如果存在，则将【】对加入结果
                    result.append(s[i:j + 1])
                    i = j + 1
            else:
                # 如果没有找到对应的】，则将【加入结果
                result.append(s[i])
                i += 1
        else:
            result.append(s[i])
            i += 1
    return ''.join(result)


# 字体转换大小写
def case_insensitive_sequence_matcher(a, b):
    # 将两个字符串都转换为小写
    a_lower = a.lower()
    b_lower = b.lower()

    # 创建 SequenceMatcher 对象
    matcher = difflib.SequenceMatcher(None, a_lower, b_lower)
    return matcher


# 一找多
def differentials(standard_string, test_data):
    standard_lower = standard_string.lower()
    matching_induce = []
    for i, item in enumerate(test_data):
        item_lower = item.lower()
        # 计算每个item与standard_string的相似度
        _, item_score = process.extractOne(item_lower, [standard_lower])
        # 只有相似度大于50的才被认为是匹配的
        if item_score > 50:
            matching_induce.append(i)

    if not matching_induce:
        return [], "", 0
    # 生成所有可能的数据排列组合
    all_permutations = list(permutations([test_data[i] for i in matching_induce]))
    max_score = 0
    best_matching_stand = ""

    for perm in all_permutations:
        joined_data = ''.join(perm)
        joined_lower = joined_data.lower()
        # 使用 SequenceMatcher 计算 standard_string 与 joined_data 之间的相似性
        matcher = difflib.SequenceMatcher(None, standard_lower, joined_lower)
        score = int(matcher.ratio() * 100)

        # 获取差异部分
        differences = []
        for tag, i1, i2, j1, j2 in matcher.get_opcodes():
            if tag in ('replace', 'delete'):
                differences.append((i1, i2))

        # 修改 standard_string，将差异部分用【】括起来
        matching_stand = ""
        last_index = 0
        for start, end in differences:
            if last_index < start:
                matching_stand += standard_string[last_index:start]
            matching_stand += f"【{standard_string[start:end]}】"
            last_index = end
        matching_stand += standard_string[last_index:]

        # 移除空的方括号
        matching_stand = remove_empty_brackets(matching_stand)

        # 更新最高相似度和最少的 stand
        if score > max_score or (score == max_score and len(differences) < best_matching_stand.count('【')):
            max_score = score
            best_matching_stand = matching_stand
    acs = True
    for strs in best_matching_stand:
        if (strs == "【") or (strs == "】"):
            acs = False
    if acs:
        max_score = 100
    return matching_induce, best_matching_stand, max_score


def fuzzy_fuzzy(standard_string, test_string):
    query = test_string.strip()
    if not query:
        return 0, "【" + standard_string + "】"
    # 获取最佳匹配结果
    best_match, score = process.extractOne(test_string, [standard_string])

    # 使用 SequenceMatcher 找出匹配的部分
    matcher = case_insensitive_sequence_matcher(standard_string, test_string)
    matching_blocks = matcher.get_matching_blocks()

    # 构建新的字符串，非匹配的部分用方括号标记
    new_string = ""
    standard_index = 0
    for block in matching_blocks:
        # 检查是否有非匹配部分需要添加
        if standard_index < block.a:
            new_string += f"【{standard_string[standard_index:block.a]}】"
        # 添加匹配部分
        new_string += standard_string[block.a:block.a + block.size]
        standard_index = block.a + block.size

    # 添加剩余的非匹配部分
    if standard_index < len(standard_string):
        new_string += f"【{standard_string[standard_index:]}】"
    if score == 100:
        new_string = standard_string
    new_string = remove_empty_brackets(new_string)
    a = True
    for strs in new_string:
        if (strs == "【") or (strs == "】"):
            a = False
    if a:
        score = 100
    new_string.encode('utf-8')
    return score, new_string


if __name__ == '__main__':
    standard = "TWI"
    test = "TwI"
    start = time.time()
    print(fuzzy_fuzzy(standard, test))
    # 一找多
    # 定义标准字符串
    stand = "DOT 052 TLTLBKDOT 052 TLTLBK xxx"
    # 定义数据列表
    data = ["135PSI	",
            "DOT 052 TLTLBK XXX	"," RADIAL""DOT052TLTLBK	",]
    print(differentials(stand, data))
    end = time.time()
    print(end - start)
