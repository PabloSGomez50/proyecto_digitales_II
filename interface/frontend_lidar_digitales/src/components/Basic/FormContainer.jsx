import { useEffect, useState } from "react";
import "./FormContainer.css";


const FormContainer = ({ data, setData, sendData, onClose }) => {

    const [valid, setValid] = useState(false);

    const renderInputs = (item) => {
        if (item.type === 'select') {
            return (
                <select name={item.id} onChange={handleChange}>
                    {item?.options.map(option => {
                        if (typeof option === 'object') {
                            return (
                                <option key={option.id} value={option.id}>
                                    {option.name}
                                </option>
                            )
                        } else {
                            return (
                                <option key={option} value={option}>
                                    {option}
                                </option>
                            )
                        }
                    })}
                </select>
            )
        } else if (item.type === 'carousel') {
            const exclude_data = data.find(iter => iter.id === item.exclude);
            return (
                <div className="carousel-container flex">
                    {/*
                    <span>
                        Anterior
                    </span>
                    */}
                    {item?.options.map(option => {
                        const disabled = exclude_data.value.includes(option.id);
                        const selected = item.value.includes(option.id);
                        return (
                        <span 
                            key={option.id}
                            onClick={() => handleCarousel({
                                selected: selected,
                                disabled: disabled,
                                name: item.id,
                                value: option.id
                            })}
                            className={"carousel-item" + (selected ? " active" : "") + (disabled ? " disabled" : "")}
                        >
                            {option.name || option.username}
                        </span>
                        )
                    }
                        
                    )}
                    {/*
                    <span>
                        Siguiente
                    </span>
                    */}

                </div>
            )
        } else {
            return (
                <input
                    type={item.type}
                    name={item.id}
                    onChange={handleChange}
                />
            )
        }
        
    }

    const handleCarousel = (data) => {
        if (data.disabled)
            return ; 
        setData(prev => {
            const index = prev.findIndex(item => item.id === data.name);
            const item = prev[index];
            const value = item.value;
            if (data.selected) {
                item["value"] = value.filter(item => item !== data.value);
            } else {
                item["value"].push(data.value);
            }
            return prev.with(index, item);
        });
    }

    const handleChange = (e) => {
        setData(prev => {

            const index = prev.findIndex(item => item.id === e.target.name);
            const item = prev[index];
            if (item.type === 'carousel') {
                item["value"].push(e.target.value);
            } else {
                item["value"] = e.target.value;
            }
            return prev.with(index, item);
        });
    }

    useEffect(() => {
        const valid = data.every(item => {
            if (item.required) {
                if (item.type === 'carousel') {
                    return item.value.length > 0;
                } else {
                    return item.value.length > 4;
                }
            }
            return true;
        });
        setValid(valid);
    }, [data]);

    const handleFormSubmit = (e) => {
        e.preventDefault();
        const device = data.reduce((acc, item) => {
            acc[item.id] = item.value;
            return acc;
        }, {});
        console.log(device);
        sendData(device);
        onClose();
    }

    return (
    <form
        className='flex-col new-duel-container form-container'
        onSubmit={handleFormSubmit}
    >
        {data.map(item => (
        <div key={item.id}>
            <label htmlFor={item.id}> 
                {item.name}:
            </label>
            {renderInputs(item)}
        </div>
        ))}

        <button
            disabled={!valid}
            className={"btn" + (valid ? " valid" : "")}
        >
            Confirmar
        </button>
    </form>
    )
}

export default FormContainer;