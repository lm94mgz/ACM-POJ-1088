#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

typedef int INDEX_TYPE;
typedef int VALUE_TYPE;

const INDEX_TYPE ERROR_INDEX = -1;

enum NEAR_TYPE{
	LEFT = 0,
	RIGHT,
	UPPER,
	LOWER,
	MAX
};

INDEX_TYPE getNearIndex(INDEX_TYPE index, NEAR_TYPE type, const INDEX_TYPE& row, const INDEX_TYPE& column)
{
	INDEX_TYPE _row = index/column, _col = index%column;

	switch (type)
	{
	case LEFT: 
		_col--;
		break;
	case RIGHT:
		_col++;
		break;
	case UPPER:
		_row --;
		break;
	case LOWER:
		_row ++;
		break;
	default:
		return ERROR_INDEX;
	}

	if (0 <= _row && _row < row && 0 <= _col && _col < column)
		return _row * column + _col;
	else
		return ERROR_INDEX;
}

int main()
{
	INDEX_TYPE row, column, sum;
	cin >> row >> column;
	sum = row * column;

	vector<VALUE_TYPE> input;
	input.resize(sum);
	for (INDEX_TYPE i = 0; i < sum; ++i)
	{
		cin >> input[i];
	}

	// 先找出所有的山顶，山顶就是比周围的点都高的点，最长滑雪距离的起点一定在山顶。
	vector<INDEX_TYPE> curVI, nextVI;
	for (INDEX_TYPE i = 0; i < sum; ++i)
	{
		int near_type;
		for (near_type = LEFT; near_type < MAX; ++near_type)
		{
			INDEX_TYPE near = getNearIndex(i, (NEAR_TYPE)near_type, row, column);
			if (near != ERROR_INDEX && input[near] > input[i])
				break;
		}

		if (near_type == MAX)
		{
			curVI.push_back(i);
		}
	}

	int result = 0;
	set<INDEX_TYPE> checked;

	// 从山顶的集合curVI出发，每次迭代尝试向周围滑一次，记录可以滑的点到集合nextVI。
	// 下一次又把nextVI中的点放入curVI继续走，这样反复迭代直到curVI为空就结束了。迭代次数就是长度。
	while (curVI.begin() != curVI.end())
	{
		for (vector<INDEX_TYPE>::iterator vi = curVI.begin(); vi != curVI.end(); ++vi)
		{
			for (int near_type = LEFT; near_type < MAX; ++near_type)
			{
				INDEX_TYPE near = getNearIndex(*vi, (NEAR_TYPE)near_type, row, column);
				if (near != ERROR_INDEX && input[near] < input[*vi])
				{
					int sub_near_type;
					for (sub_near_type = LEFT; sub_near_type < MAX; ++sub_near_type)
					{
						INDEX_TYPE sub_near = getNearIndex(near, (NEAR_TYPE)sub_near_type, row, column);
						if (sub_near != ERROR_INDEX && sub_near != *vi 
								&& input[sub_near] > input[near] && checked.find(sub_near) == checked.end())	
							// 重要的剪枝：
							// 如果将要滑到的一个目标点(near)，它的周围还有除了出发点(*vi)之外的更高点，即还有其他点(sub_near)可以滑向它，
							// 那么对于其他点(sub_near)，有三种情况：
							// 1. 其他点(sub_near)已经在checked表中，说明从其他点滑过去更近，那么从当前点过去就是一条更远的值得走的路；
							// 2. 其他点(sub_near)在curVI表中，说明从其他点滑过去和从当前点滑过去一样近，两种情况中处理一种即可；
							// 3. 其他点(sub_near)既不在checked表中也不在curVI中，说明从其他点滑过去比当前点滑过去更远，那么当前不处理该点，所以要break;
							break;
					}
					if (sub_near_type == MAX)
					{
						nextVI.push_back(near);
					}
				}
			}

			checked.insert(*vi);
		}

		result++;
		curVI.swap(nextVI);
		nextVI.clear();
	}

	cout << result << endl;
	return 0;
}
