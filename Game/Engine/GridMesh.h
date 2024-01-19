#pragma once
//2024-01-18 �̼���
//�ϳ��� �޽����� ����ȭ�� �׸���޽�.

class GridMesh : public CMesh
{
public:
    GridMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,
        int nWidth, int nLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
    virtual ~GridMesh();

    XMFLOAT3 GetScale() { return(m_xmf3Scale); }
    int GetWidth() { return(m_nWidth); }
    int GetLength() { return(m_nLength); }


protected:
    int m_nWidth;
    int m_nLength;
    XMFLOAT3 m_xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
};
